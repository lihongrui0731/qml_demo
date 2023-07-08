import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qmlElement/chart"
import "../charts"

Item {
    id: stream
//    anchors.fill: parent
    width: parent.width - parent.anchors.margins
    height: parent.height - parent.anchors.margins
    function dataReceived(data) {
        console.log("stream page: ", data)
    }

    Connections {
        target: root
        function onRecord() {
            record()
        }
        function onStop() {
            stop()
        }
    }

    Column {
        anchors.fill: parent
        spacing: 5
        Row {
            spacing: 5
            width: stream.width
            height: (stream.height - parent.spacing) * 0.5
            EvChartBox {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
            }
            FFTChartBox {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
            }
        }
        Row {
            spacing: 5
            width: stream.width
            height: (stream.height - parent.spacing) * 0.5
            WaveChartBox {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
            }
            PrpdChartBox {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
            }
        }
    }
}
