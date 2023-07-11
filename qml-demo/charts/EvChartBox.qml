import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/qmlElement/chart"
import "../components"
import "../config/evTypes.js" as EvTypes

Item {
    id: evBox
    property var evTypes: EvTypes["soundEvTypes"]
    property string evType: "Level_A"
    signal setEvType(type: string)
    function changeEvType(type) {
        console.log("ev type changing to: ", type, "from: ", evType)
        if(type === "Level_A" && type !== evType) {
            evChartLoader.sourceComponent = leqChartComponent
        }
        if(type !== "Level_A" && evType === "Level_A") {
            evChartLoader.sourceComponent = evChartComponent
        }
        if(type !== "Level_A") setEvType(type)
        evType = type
    }

    Component.onCompleted: {
        evTypes = EvTypes["soundEvTypes"]
    }

    Card {
        id: evChartBox
        anchors.fill: parent
        title: "特征值"
        property var cardContentPosition
        property var cardHeaderPosition
        property var cardHeaderExtraPosition
        Component.onCompleted: {
            cardContentPosition = evChartBox.getContentPosition()
            cardHeaderPosition = evChartBox.getCardHeaderPosition()
            cardHeaderExtraPosition = evChartBox.getCardHeaderExrtraPosition()
        }

//        Connections {
//            target: webSocketManager
//            function onLeqDataReceived(leqData) {
//                leqData.dt = 1 / leqData.values.length
//                leqChart.setLeqData("sound", JSON.stringify(leqData))
//            }
//            function onEvDataReceived(evData) {
//                evChart.eigenvalueChanged("sound", evData)
//            }
//        }

        Item {
            id: cardHeaderExtra
            y: evChartBox.cardHeaderExtraPosition.y
            anchors.right: parent.right
            width: parent.width / 2
            height: evChartBox.cardHeaderExtraPosition.height
            Component.onCompleted: {
                evTypeSelection.currentIndex = 0
            }

            ComboBox {
                id: evTypeSelection
                height: parent.height
                width: 150
                anchors.right: parent.right
                textRole: "label"
                valueRole: "value"
                model: evTypes
                onActivated: {
                    var type = evTypes[index]["value"]
                    changeEvType(type)
                }
            }
        }
        Loader {
            id: evChartLoader
            sourceComponent: leqChartComponent
            anchors.margins: 5
            x: evChartBox.cardContentPosition.x
            y: evChartBox.cardContentPosition.y
            width: evChartBox.cardContentPosition.width - evChartLoader.anchors.margins
            height: evChartBox.cardContentPosition.height - evChartLoader.anchors.margins
//            onLoaded: {
//                leqChart.initChart()
//            }
        }

        Component {
            id: leqChartComponent
            LeqChart {
                id: leqChart
                anchors.fill: parent
                Component.onCompleted: {
                    leqChart.initChart()
                }

                Connections {
                    target: webSocketManager
                    function onLeqDataReceived(leqData) {
                        leqData.dt = 1 / leqData.values.length
                        leqChart.setLeqData("sound", JSON.stringify(leqData))
                    }
                }
                Connections {
                    target: root
                    function onChangeCurrentChannel(ch) {
                        if(evType === "Level_A") leqChart.initChart()
                    }
                }
            }
        }
        Component {
            id: evChartComponent
            SpChart {
                id: evChart
                anchors.fill: parent
                Component.onCompleted: {
                    evChart.initChart()
                }

                Connections {
                    target: webSocketManager
                    function onEvDataReceived(evData) {
                        evChart.eigenvalueChanged("sound", JSON.stringify(evData))
                    }
                }
                Connections {
                    target: evBox
                    function onSetEvType(type) {
//                        console.log("-------------set type", type)
                        evChart.setType(type, "")
                    }
                }
                Connections {
                    target: root
                    function onChangeCurrentChannel(ch) {
                        if(evType !== "Level_A") evChart.initChart()
                    }
                }
            }
        }
    }
}
