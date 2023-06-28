#ifndef WS_SERVER_H
#define WS_SERVER_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QSettings>
#include <QtWebSockets>
#include <QJsonObject>
#include <QTimer>

class WsServer : public QObject
{
    Q_OBJECT

public:
    explicit WsServer(QObject *parent = nullptr);

    virtual ~WsServer() override;

    void setCalRate( bool isCalRate );

public slots:

    /// 向当前连入的 client 发送文本消息
    qint64 sendOutgoingTextMessage(const QString& message);

    /// 向当前连入的 client 发送字节流数据消息
    qint64 sendOutgoingBinaryMessage(const QByteArray& data);

signals:

    /// 已接受一个传入连接
    void clientAccepted( QString clientId );

    /// 收到客户端发来的文本消息
    void incomingTextMessageReceived(const QString& message);

    /// 收到客户端发来的字节流消息
    void incomingBinaryMessageReceived(const QByteArray& message);

    void sendIpAddressRate( QHash<QString , quint64>& rate );

    /// WebSocket 服务器主动关闭
    void serverClosed();

private slots:

    /** 收到来自 client 的连接请求 */
    void onNewConnection();
    /** client 已断开连接 */
    void clientDisconnected();

    /** 收到来自client的文本消息 */
    void onTextMessage(const QString& message);

    /** 收到来自client的数据消息 */
    void onBinaryMessage(const QByteArray& message);

    void checkRate();

private:

    // 外部依赖

    QWebSocketServer *m_pWebSocketServer;

    // 配置选项

    /// 是否打印调试消息
    bool m_debug{true};

    /// WebSocket 监听端口
    quint16 m_port{6380};

    // 运行状态

    QTimer *m_timer{nullptr};
    bool isCalRate{false};

    int m_lastMsgID{0};

    // 已接受的传入连接
    QList<QWebSocket *> m_clients{};

    QVector<QString> m_sources;
    QHash<QString , QWebSocket*> m_clientsSource{};
    // utils

    QHash<QString , quint64> m_clients_rate{};

    void logDebug(const QString& msg) {
        if (!m_debug) return;
        qDebug().noquote() << "WsServer -" << msg;
    }

    QString clientId(void* pClient) {
        return QString("0x%1").arg((quintptr)pClient, 8, 16, QLatin1Char('0'));
    }

};

#endif // WS_SERVER_H
