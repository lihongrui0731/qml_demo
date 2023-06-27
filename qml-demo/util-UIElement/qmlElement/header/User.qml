import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: user_info
    Button{
        id: user_btn
        anchors.right: user_info.right
        height:45/root.dpi
        width: 60/root.dpi

        Label {
            id: user_btn_label
            text: qsTr("登录")
            color: "blue"
            anchors.centerIn: parent
            font.pixelSize: 16/root.dpi
        }

        background: Rectangle {
            color: Qt.rgba(242/255,242/255,242/255,0)
        }
        onClicked:{
            console.log( " login" )
        }

        MouseArea{
            anchors.fill: parent;
            hoverEnabled: true;
            cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
            acceptedButtons: Qt.NoButton
        }
    }

    Label {
        id: user_label
        text: qsTr("当前用户:")
        font.pixelSize: 16/root.dpi
        anchors.right: user_btn.left
        anchors.verticalCenter: parent.verticalCenter
    }
}
