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


            Loader {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
                sourceComponent: rect
            }

        }
        Row {
            spacing: 5
            width: stream.width
            height: (stream.height - parent.spacing) * 0.5
            Loader {
                height: parent.height
                width: (parent.width - parent.spacing) * 0.5
                sourceComponent: rect
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
            color:"#595b5d"
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
}
