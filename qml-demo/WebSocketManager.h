#ifndef WEBSOCKETMANAGER_H
#define WEBSOCKETMANAGER_H

#include <QObject>
#include "./ws_server.h"

class WebSocketManager : public QObject
{
    Q_OBJECT
public:
    WebSocketManager();
    ~WebSocketManager();

private:
    WsServer * WebSocketServer;
    QThread* wsServerThread;

signals:
    void clientAccepted(QString clientId);
    void incomingTextMessageReceived(const QString& message);

public slots:
    // Receiving
    void onClientAccepted(QString clientId);
    void onIncomingTextMessageReceived(const QString& message);

    // Sending
    void sendTextMessage(const QString message);
};


#endif // WEBSOCKETMANAGER_H
