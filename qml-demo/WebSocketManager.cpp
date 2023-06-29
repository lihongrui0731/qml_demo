#include "WebSocketManager.h"

WebSocketManager::WebSocketManager() {
    WebSocketServer = new WsServer;

    connect(WebSocketServer, &WsServer::clientAccepted, this, &WebSocketManager::onClientAccepted);
    connect(WebSocketServer, &WsServer::incomingTextMessageReceived, this, &WebSocketManager::onIncomingTextMessageReceived);
}

void WebSocketManager::onClientAccepted(QString clientId) {
    emit clientAccepted(clientId);
}

void WebSocketManager::onIncomingTextMessageReceived(const QString& message) {
    emit incomingTextMessageReceived(message);
}

void WebSocketManager::sendTextMessage(QString message) {
    WebSocketServer->sendOutgoingTextMessage(message);
}
