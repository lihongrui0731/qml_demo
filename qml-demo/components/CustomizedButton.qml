import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: customizedButton
    property string text
    property real fontSize
    anchors.verticalCenter: parent.verticalCenter
    height: parent.height - parent.spacing
    width: 50
    color: root.cardColor
    signal clicked()

    Label {
        anchors.centerIn: parent
        text: qsTr(customizedButton.text)
        font.pixelSize: fontSize
    }
    MouseArea {
        anchors.fill: parent
        onClicked: customizedButton.clicked()
    }
}
