import QtQuick 2.15
import QtQuick.Controls 2.15
//import QtQuick.Dialogs 1.2

Dialog {
    title: "采集配置"
    height: root.height * 0.7
    width: root.width * 0.4
    anchors.centerIn: parent
    standardButtons: Dialog.Ok | Dialog.Cancel

    Rectangle {
        id: deviceConfigContent
        anchors.fill: parent
        color: "lightpink"
    }

    onAccepted: console.log("saved!")

    function openDeviceConfig() {
        let a = {
            leqEn: true,
            wavEn: false
        }
        let s = JSON.stringify(a)
        console.log(s)
        deviceConfig.open()
    }
}

