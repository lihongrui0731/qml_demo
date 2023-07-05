#include "ws_server.h"

#include <QCoreApplication>
#include <QDebug>

WsServer::WsServer(QObject *parent)
    : QObject{ parent }
    , m_pWebSocketServer{new QWebSocketServer{QCoreApplication::applicationName(),
                         QWebSocketServer::NonSecureMode, this}}
{
    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName() };
    m_port = static_cast<quint16>(settings.value("server/wsPort", 8000).toUInt());
    if (m_pWebSocketServer->listen(QHostAddress::Any, m_port)) {
        logDebug(QString("Listening on port %1").arg(m_port));
    } else {
        logDebug("WebSocket server listen failed");
    }

    connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &WsServer::onNewConnection);
    connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WsServer::serverClosed);

    connect(m_pWebSocketServer, &QWebSocketServer::acceptError , [](QAbstractSocket::SocketError err_ ){
            qDebug() << err_ << " websocket server accept error";
    });

    connect(m_pWebSocketServer, &QWebSocketServer::serverError , [](QWebSocketProtocol::CloseCode err_ ){
            qDebug() << err_ << " websocket server CloseCode error";
    });

    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout , this, &WsServer::checkRate);
}

WsServer::~WsServer()
{
    logDebug(__func__);
    m_pWebSocketServer->close();
    // DEBUG core dump below?
    // qDeleteAll(m_clientsSource.begin(),m_clientsSource.end());
}

void WsServer::setCalRate( bool isCalRate_ ){
    isCalRate = isCalRate_;
    m_timer->start(2000);
}

void WsServer::checkRate(){
    emit sendIpAddressRate(m_clients_rate);

    QHash<QString , quint64>::Iterator iter = m_clients_rate.begin();
    while( iter != m_clients_rate.end() ){
        iter.value() = 0;
        iter++;
    }
}

void WsServer::onNewConnection()
{
    QWebSocket *pClient= m_pWebSocketServer->nextPendingConnection();

    QString clientId_ = clientId(pClient);
    logDebug(QString("client accepted: %1").arg(clientId(pClient)));

    // 目前实现为只保留一个传入连接
    // 若改为支持多个连接，要维护各客户端不同的请求序号
    /*
    if ( m_clients.size() > 0) {
        for (int i{0}; i < m_clients.size(); i++) {
            if (m_clients[i]->state() == QAbstractSocket::ConnectedState)
                m_clients[i]->close();
        }
        qDeleteAll(m_clients);
        m_clients.clear();
    }
    */
    QString ip_ = pClient->peerAddress().toString().mid(7);

    if( m_clientsSource.keys().contains(ip_) ){
        /*
        int len = m_clients.length();
        for( int i = 0; i < len; i++ ){
            if( m_clients[i]->peerAddress().toString() == pClient->peerAddress().toString() ){
                m_clients.removeAt(i);
            }
        }
        */

        m_clientsSource[ip_]->close();
        m_clientsSource.remove(ip_);
    }

    m_clientsSource.insert( ip_ , pClient);
    // m_clients.append(pClient);

//    qDebug() << pClient->peerAddress().toString().mid(7)<< "ipaddress";

    m_clients_rate.insert( ip_ , 0 );

    // qDebug() << pClient->peerAddress().toString().mid(7) << "VVVVVVVV";
    // 重置消息序号
    m_lastMsgID = 0;

    connect(pClient, &QWebSocket::textMessageReceived, this ,&WsServer::onTextMessage);
    connect(pClient, &QWebSocket::binaryMessageReceived, this, &WsServer::onBinaryMessage);
    connect(pClient, &QWebSocket::disconnected, this, &WsServer::clientDisconnected);

    emit clientAccepted( clientId_ );
}

void WsServer::clientDisconnected()
{
    // 客户方主动断开连接
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug("WsServer - client disconnected: %p", static_cast<void*>(pClient));

    qDebug() << pClient->errorString() << "disconnect error";

    QString ip_ = pClient->peerAddress().toString().mid(7);
    if( m_clientsSource.contains(ip_) ) {
        /*
        int len = m_clients.size();
        for( int i = 0; i < len; i++ ){
            m_clients.removeAt(i);
        }
        */
        m_clientsSource[ip_]->close();
        m_clientsSource.remove(ip_);
        //pClient->deleteLater();
    }
}

void WsServer::onTextMessage(const QString& message)
{
    if( message.isEmpty() ){
        return;
    }

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QString ipAddress = pClient->peerAddress().toString().mid(7);

    quint64 rate = m_clients_rate.value(ipAddress);
    rate += message.size() * sizeof(QString);

    m_clients_rate.insert(ipAddress,rate);

    // logDebug(QString("Text msg received (from %1)").arg(clientId(sender())));
    emit incomingTextMessageReceived(message);
}

void WsServer::onBinaryMessage(const QByteArray& message)
{
    if( message.isEmpty() ){
        return;
    }

//    logDebug(QString("Binary msg received (from %1)").arg(clientId(sender())));
    /*
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
    */
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QString ipAddress = pClient->peerAddress().toString().mid(7);

    quint64 rate = m_clients_rate.value(ipAddress);
    rate += message.size();

    m_clients_rate.insert(ipAddress,rate);

    emit incomingBinaryMessageReceived(message);
}

qint64 WsServer::sendOutgoingTextMessage(const QString &message)
{
    QJsonParseError jerr_;
    QJsonDocument jdoc_ = QJsonDocument::fromJson(message.toLatin1(),&jerr_);
    if( jerr_.error != QJsonParseError::NoError ){
        qDebug() << "json data error:" << jerr_.error;
    }
    QJsonObject jobc = jdoc_.object();
    qint64 length{0};

    if( jobc.contains("source") ){
        QString source = jobc["source"].toString();
        if( !m_clientsSource.contains(source) ) return length;

        length = m_clientsSource[source]->sendTextMessage(message);
    }else{
        qDebug() << m_clientsSource.size() << "clients size --- ";

        QHash<QString , QWebSocket*>::Iterator iter = m_clientsSource.begin();
        while( iter != m_clientsSource.end() ){
            iter.value()->sendTextMessage(message);
            iter++;
        }

/*
        for (int i{0}; i < m_clients.size(); i++) {
            length = m_clients[i]->sendTextMessage(message);
        }
 */
   }
    return length;
}

qint64 WsServer::sendOutgoingBinaryMessage(const QByteArray &data)
{
    qint64 length{0};
    QHash<QString , QWebSocket*>::Iterator iter = m_clientsSource.begin();
    while( iter != m_clientsSource.end() ){
        iter.value()->sendBinaryMessage(data);
        iter++;
    }

    /*
    for (int i{0}; i < m_clients.size(); i++) {
        length = m_clients[i]->sendBinaryMessage(data);
    }
    */
    return length;
}
