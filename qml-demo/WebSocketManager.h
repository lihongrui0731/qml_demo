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

    // Sending
    Q_INVOKABLE void sendTextMessage(const QString message);
private:
    WsServer * WebSocketServer;
    QThread* wsServerThread;

signals:
    void clientAccepted(QString clientId);
    void incomingTextMessageReceived(const QString& message);
    void sendOutgoingTextMessage(const QString& message);

public slots:
    // Receiving
    void onClientAccepted(QString clientId);
    void onIncomingTextMessageReceived(const QString& message);

};


#endif // WEBSOCKETMANAGER_H
