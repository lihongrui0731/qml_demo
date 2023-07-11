import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qmlElement/chart"
import "../components"

Item {
    Card {
        id: fftChartBox
        anchors.fill: parent
        title: "FFT"
        property var cardContentPosition
        Component.onCompleted: {
            cardContentPosition = fftChartBox.getContentPosition()
            fftChart.initChart()
        }

        Connections {
            target: webSocketManager
            function onFftDataReceived(fftData) {
                fftChart.addSpectraData("sound", fftData)
            }
        }
        Connections {
            target: root
            function onChangeCurrentChannel(ch) {
                fftChart.initChart()
            }
        }

        FFTChart {
            id: fftChart
            anchors.margins: 5
            x: fftChartBox.cardContentPosition.x
            y: fftChartBox.cardContentPosition.y
            width: fftChartBox.cardContentPosition.width - fftChart.anchors.margins
            height: fftChartBox.cardContentPosition.height - fftChart.anchors.margins
        }
    }
}
