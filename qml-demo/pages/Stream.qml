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

    signal stop()
    signal record()
    signal leqDataReceived(string channelId, string data)
    signal fftDataReceived(string channelId, var data)
    signal prpdDataReceived(var data)
    Connections {
        target: root
        function onRecord() {
            record()
        }
        function onStop() {
            stop()
        }
    }
    Connections {
        target: webSocketManager
        function onLeqDataReceived(leqData) {
            leqData.dt = 1 / leqData.values.length
            var dataStr = JSON.stringify(leqData)
            leqDataReceived("sound", dataStr)
        }
        function onFftDataReceived(fftData) {
            fftDataReceived("sound", fftData)
        }
        function onPrpdDataReceived(prpdData) {
            prpdDataReceived(prpdData)
        }
    }

    Column {
        anchors.fill: parent
        spacing: 5
        Row {
            spacing: 5
            width: stream.width
            height: (stream.height - parent.spacing) * 0.5
            LeqChartBox {
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

    Component {
        id: rect
        Rectangle {
            anchors.fill: parent
//            color:"#595b5d"
            color: root.cardColor
            radius: 5
        }
    }

    Component {
        id: leqBox
        Loader {
            sourceComponent: rect
            height: parent.height
            width: (parent.width - parent.spacing) * 0.5
        }

    }
    Component {
        id: fftBox
        Loader {
            sourceComponent: rect
            height: parent.height
            width: (parent.width - parent.spacing) * 0.5
        }
    }
    Component {
        id: waveBox
        Loader {
            sourceComponent: rect
            height: parent.height
            width: (parent.width - parent.spacing) * 0.5
        }
    }
}
