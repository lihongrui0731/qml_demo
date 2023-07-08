import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    anchors.fill: parent
    color: root.cardColor
    property string title: ""
    function getContentPosition() {
        return {
            "x": cardContent.x,
            "y": cardContent.y,
            "width": cardContent.width,
            "height": cardContent.height
        }
    }
    function getCardHeaderPosition() {
        return {
            "x": cardHeader.x,
            "y": cardHeader.y,
            "width": cardHeader.width,
            "height": cardHeader.height
        }
    }
    function getCardHeaderExrtraPosition() {
        return {
            "x": cardHeader.x + name.width,
            "y": cardHeader.y,
            "width": cardHeader.width - name.width,
            "height": card.cardHeaderHeight
        }
    }

    Item {
        id: card
        anchors.fill: parent
        property real cardHeaderHeight: 30
        property real margins: 5

        Rectangle {
            id: cardHeader
            width: parent.width
            height: card.cardHeaderHeight
            radius: 5
            Text {
                id: name
                text: qsTr(title)
                font.pixelSize: 14/root.dpi
//                x: card.margins
                anchors.left: parent.left
                anchors.leftMargin: card.margins
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            id: cardContent
            x: card.margins
            y: cardHeader.height
            width: parent.width - card.margins
            height: parent.height - cardHeader.height
            color: root.cardColor
            radius: 5
//            z: cardHeader.z + 1
        }
    }
}
