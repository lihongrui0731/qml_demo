import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qmlElement/chart"
import "../components"

Item {
    Card {
        id: waveChartBox
        anchors.fill: parent
        title: "wave"
        property var cardContentPosition
        Component.onCompleted: {
            cardContentPosition = waveChartBox.getContentPosition()
        }

        Connections {
            target: webSocketManager
            function onWaveDataReceived(data) {
                waveChart.setJsonData("sound", data["1"])
            }
        }

        TimedataChart {
            id: waveChart
            anchors.margins: 5
            x: waveChartBox.cardContentPosition.x
            y: waveChartBox.cardContentPosition.y
            width: waveChartBox.cardContentPosition.width - waveChart.anchors.margins
            height: waveChartBox.cardContentPosition.height - waveChart.anchors.margins
            channelId: "sound"
        }
    }
}
