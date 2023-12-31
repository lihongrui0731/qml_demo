#include "WebSocketManager.h"
//#include "./src/dataType.h"

// construction
WebSocketManager::WebSocketManager()
{
    WebSocketServer = new WsServer;

    wsServerThread = new QThread();
    WebSocketServer->moveToThread(wsServerThread);

    connect(WebSocketServer, &WsServer::clientAccepted, this, &WebSocketManager::onClientAccepted);
    connect(WebSocketServer, &WsServer::incomingTextMessageReceived, this, &WebSocketManager::onIncomingTextMessageReceived);
    connect(WebSocketServer, &WsServer::incomingBinaryMessageReceived, this, &WebSocketManager::onIncomingBinaryMessageReceived);
    connect(this, &WebSocketManager::sendOutgoingTextMessage, WebSocketServer, &WsServer::sendOutgoingTextMessage);
    wsServerThread->start();
}

// distruction
WebSocketManager::~WebSocketManager()
{
    if(wsServerThread != nullptr) {
        wsServerThread->quit();
        wsServerThread->wait();
        delete wsServerThread;
        wsServerThread = nullptr;
    }
}

void WebSocketManager::onClientAccepted(QString clientId)
{
    emit clientAccepted(clientId);
}

void WebSocketManager::onIncomingTextMessageReceived(const QString& message)
{
//    qDebug() << "WebSocketManager - " << "message arrived";
    emit incomingTextMessageReceived(message);
    messageReceived(message);
}

void WebSocketManager::onIncomingBinaryMessageReceived(const QByteArray& message)
{
//    qDebug() << "binary received" << message.size();
    handleWaveData(message);
}

void WebSocketManager::sendTextMessage(QString message)
{
    //    WebSocketServer->sendOutgoingTextMessage(message);
    qDebug() << "sending message";
    emit sendOutgoingTextMessage(message);
}


void WebSocketManager::messageReceived(const QString& textData)
{
    QJsonDocument jdoc = QJsonDocument::fromJson(textData.toLatin1());
    QJsonObject dataObj = jdoc.object();
    QJsonObject params = dataObj["params"].toObject();
    if(dataObj["method"] == "deviceID") {
        QString deviceID = params["deviceID"].toString();
        handleDeviceID(deviceID);
    }
    if(dataObj["method"] == "dataFrame") {
        handleDataFrame(dataObj["params"].toObject());
    }
}
void WebSocketManager::handleDeviceID(const QString& deviceID)
{
    qDebug() << deviceID << "get in";
    emit deviceIDReceived(deviceID);
}
void WebSocketManager::handleDataFrame(const QJsonObject& data)
{
    QString frameType = data["frameType"].toString();
    if(frameType == "Level_A") {
        qDebug() << "leq received";
        emit leqDataReceived(data["data"].toObject());
    }
    if(frameType == "ev-sp") {
        qDebug() << "ev-sp received";
        emit evDataReceived(data["data"].toObject());
    }
    if(frameType == "spectrum") {
        qDebug() << "fft received";
        emit fftDataReceived(data["data"].toObject());
    }
    if(frameType == "PRPD") {
        qDebug() << "prpd received";
        emit prpdDataReceived(data["data"].toObject());
    }
}

void WebSocketManager::handleWaveData(const QByteArray& data)
{
    QDataStream reader(data);
    setupStreamReader(reader);
    QScopedPointer<HeaderInfo> header { new HeaderInfo };
    reader >> header->timestamp >> header->channelFlag;
//    reader.skipRawData(7);
    QList<int> channels;
    int channel{};
    quint32 mask{1};
    for (int ch {1}; ch <= (int)(sizeof(quint32))<<1; ++ch, mask <<= 1) {
        if (header->channelFlag & mask) {
            channels.push_back(ch);
        }
    }

//    qDebug() << channel;
    int byteLength { data.size() - 12 };
    int dataArrayLength { byteLength / (int)(sizeof(qint32)) };

    if(m_maintainWave.isEmpty()) {
        m_maintainWave.reserve(channels.size());
        for ( const int ch : channels ){
            QVector<qint32> value;
            value.reserve( dataArrayLength / channels.size() );
            m_maintainWave.insert(ch ,value);
        }
    }

    for (int i{0}; i < dataArrayLength; ++i) {
        qint32 value;
        reader >> value;

        if(m_maintainWave[channels.at( i % channels.size() )].size() < m_maintainWaveLength){
            QVector<qint32>& value_ = m_maintainWave[ channels.at( i % channels.size() ) ];
            value_.push_back( value );
            m_maintainWave.insert( channels.at( i % channels.size() ) , value_ );
        } else {
            QJsonObject outData;
            QJsonObject channelData;
            channelData.insert("values", toJsonArray(m_maintainWave[channels.at( i % channels.size() )]));
            channelData.insert("dt", (1.0/48000.0));
            outData.insert(QString::number(channels.at( i % channels.size() )), channelData);
            qDebug() << channels.at( i % channels.size() ) << outData[QString::number(channels.at( i % channels.size() ))].toObject()["values"].toArray().size();
            emit waveDataReceived(outData);

            m_maintainWave[channels.at( i % channels.size() )].clear();
            QVector<qint32>& value_ = m_maintainWave[ channels.at( i % channels.size() ) ];
            value_.push_back( value );
            m_maintainWave.insert( channels.at( i % channels.size() ) , value_ );
        }
    }

//    for ( const int ch : channels ){
//        emit channelSamples(ch, channelsData.value(ch));
//        //qDebug() << "ch:" << ch << "length:" << channelsData.value(ch).size();
//    }

//    for (int i{0}; i<dataArrayLength; i++) {
//        qint32 value;
//        reader >> value;
//        if(m_maintainWave[QString::number(channel)].size() < m_maintainWaveLength) {
//            m_maintainWave[QString::number(channel)].append(value);
//        } else {
//            QJsonObject outData;
//            QJsonObject channelData;
//            channelData.insert("values", m_maintainWave[QString::number(channel)]);
//            channelData.insert("dt", (1.0/48000.0));
//            outData.insert(QString::number(channel), channelData);
//            qDebug() << channel << outData[QString::number(channel)].toObject()["values"].toArray().size();
//            emit waveDataReceived(outData);

//            m_maintainWave[QString::number(channel)] = QJsonArray();
//            m_maintainWave[QString::number(channel)].append(value);
//        }
//    }
}
void WebSocketManager::setupStreamReader(QDataStream& reader)
{
    reader.setVersion(QDataStream::Qt_5_12);
    reader.setByteOrder(QDataStream::LittleEndian);
    reader.setFloatingPointPrecision(QDataStream::SinglePrecision);
}

