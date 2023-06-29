import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qmlElement/chart"
import "../components"

Item {
    Card {
        id: prpdChartBox
        anchors.fill: parent
        title: "PRPD"
        property var cardContentPosition
        Component.onCompleted: {
            cardContentPosition = prpdChartBox.getContentPosition()
            console.log(cardContentPosition.width, cardContentPosition.height)
        }

        PrpdChart {
            id: prpdChart
            anchors.margins: 5
            x: prpdChartBox.cardContentPosition.x
            y: prpdChartBox.cardContentPosition.y
            width: prpdChartBox.cardContentPosition.width - prpdChart.anchors.margins
            height: prpdChartBox.cardContentPosition.height - prpdChart.anchors.margins
        }
    }
}
