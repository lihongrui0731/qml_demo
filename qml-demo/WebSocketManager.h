#ifndef WEBSOCKETMANAGER_H
#define WEBSOCKETMANAGER_H

#include <QObject>
#include "./ws_server.h"

class WebSocketManager : public QObject
{
    Q_OBJECT
public:
    WebSocketManager();

private:
    WsServer* WebSocketServer;
};


#endif // WEBSOCKETMANAGER_H
