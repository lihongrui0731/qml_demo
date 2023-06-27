import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qmlElement/chart"

Item {
    id: stream
//    anchors.fill: parent
    width: parent.width - parent.anchors.margins
    height: parent.height - parent.anchors.margins
    Column {
        anchors.fill: parent
        spacing: 5
        Row {
            spacing: 5
            width: stream.width
            height: (stream.height - parent.spacing) * 0.5
//            Loader {
//                height: parent.height
//                width: (parent.width - parent.spacing) * 0.5
//                sourceComponent: rect

//            }
            Item {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
                Loader {
//                    height: parent.height
//                    width: (parent.width - parent.spacing) * 0.5
                    anchors.fill: parent
                    sourceComponent: rect
                }
                LeqChart {
                    anchors.top: parent.anchors.top
                    height: parent.height - 8
                    width: parent.width - 8
                    anchors.centerIn: parent
                }
            }


            Item {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
                Loader {
//                    height: parent.height
//                    width: (parent.width - parent.spacing) * 0.5
                    anchors.fill: parent
                    sourceComponent: rect
                }
                SpectrumChart {
                    anchors.top: parent.anchors.top
                    height: parent.height - 8
                    width: parent.width - 8
                    anchors.centerIn: parent
                }
            }

        }
        Row {
            spacing: 5
            width: stream.width
            height: (stream.height - parent.spacing) * 0.5
            Item {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
                Loader {
//                    height: parent.height
//                    width: (parent.width - parent.spacing) * 0.5
                    anchors.fill: parent
                    sourceComponent: rect
                }
                TimedataChart {
                    anchors.top: parent.anchors.top
                    height: parent.height - 8
                    width: parent.width - 8
                    anchors.centerIn: parent
                }
            }
            Loader {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
                sourceComponent: rect
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
        id: card
        Column {
            spacing: 0
            anchors.fill: parent
            Rectangle {
                id: cardHeader
                width: parent.width
                height: 20
            }
            Rectangle {
                id: cardContent
                width: parent.width
                height: parent.height - cardHeader.height
            }
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
