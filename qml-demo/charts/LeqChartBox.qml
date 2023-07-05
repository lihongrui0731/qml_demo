import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qmlElement/chart"
import "../components"

Item {
    Card {
        id: leqChartBox
        anchors.fill: parent
        title: "Leq(A)"
        property var cardContentPosition
        Component.onCompleted: {
            cardContentPosition = leqChartBox.getContentPosition()
            console.log(cardContentPosition.width, cardContentPosition.height)
        }

        Connections {
            target: webSocketManager
            function onLeqDataReceived(leqData) {
                leqData.dt = 1 / leqData.values.length
                leqChart.setLeqData("sound", JSON.stringify(leqData))
            }
        }

        LeqChart {
            id: leqChart
            anchors.margins: 5
            x: leqChartBox.cardContentPosition.x
            y: leqChartBox.cardContentPosition.y
            width: leqChartBox.cardContentPosition.width - leqChart.anchors.margins
            height: leqChartBox.cardContentPosition.height - leqChart.anchors.margins
        }
    }
}
