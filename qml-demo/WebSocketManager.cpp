#include "WebSocketManager.h"

// construction
WebSocketManager::WebSocketManager() {
    WebSocketServer = new WsServer;

    wsServerThread = new QThread();
    WebSocketServer->moveToThread(wsServerThread);

    connect(WebSocketServer, &WsServer::clientAccepted, this, &WebSocketManager::onClientAccepted);
    connect(WebSocketServer, &WsServer::incomingTextMessageReceived, this, &WebSocketManager::onIncomingTextMessageReceived);
    connect(this, &WebSocketManager::sendOutgoingTextMessage, WebSocketServer, &WsServer::sendOutgoingTextMessage);
    wsServerThread->start();
}

// distruction
WebSocketManager::~WebSocketManager() {
    if(wsServerThread != nullptr) {
        wsServerThread->quit();
        wsServerThread->wait();
        delete wsServerThread;
        wsServerThread = nullptr;
    }
}

void WebSocketManager::onClientAccepted(QString clientId) {
    emit clientAccepted(clientId);
}

void WebSocketManager::onIncomingTextMessageReceived(const QString& message) {
    qDebug() << "WebSocketManager - " << message;
    emit incomingTextMessageReceived(message);
}

void WebSocketManager::sendTextMessage(QString message) {
//    WebSocketServer->sendOutgoingTextMessage(message);
    qDebug() << "sending message";
    emit sendOutgoingTextMessage(message);
}
