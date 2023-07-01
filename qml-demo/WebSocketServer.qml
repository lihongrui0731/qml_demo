import QtQuick 2.15
import WebSocketManager 1.0


Item {
    WebSocketManager {
        id: webSocketManager
        objectName: "webSocketManager"
    }
    Connections {
        target: streamPage
        onClientAccepted: {
            console.log(clientId)
        }
        onIncomingTextMessageReceived: {
            console.log("WebSocketManager: ", message)
        }
    }
}

