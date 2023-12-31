import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qmlElement/chart"
import "../components"

Item {
    Card {
        id: waveChartBox
        anchors.fill: parent
        title: "时域波形"
        property var cardContentPosition
        property string channel: "1"
        Component.onCompleted: {
            cardContentPosition = waveChartBox.getContentPosition()
            waveChart.reflash()
        }

        Connections {
            target: webSocketManager
            function onWaveDataReceived(data) {
                waveChart.setJsonData("sound", data[waveChartBox.channel])
            }
        }
        Connections {
            target: root
            function onChangeCurrentChannel(ch) {
                waveChart.reflash()
                waveChartBox.channel = ch.toString()
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
