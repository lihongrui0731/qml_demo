import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Controls.Material 2.12

import FileManager 1.0

import "config/deviceConfig.js" as Device_config

ApplicationWindow {
    id: root
    visible: true
    width: 1280
    height: 750
    title: qsTr("Application")
    //    background: Rectangle {
    //        id: rootBackground
    //        color: "#202123"
    //    }
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    property var deviceConfig: Device_config

    menuBar: MenuBar {
        Menu {
            id: fileMenu
            width: 150
            title: "文件"

            MenuItem {
                height: 30
                text: "新建采集"
                onTriggered: {
                    pageLoader.sourceComponent = streamPage
                }
            }
            MenuItem {
                height: 30
                text: "打开"

                onTriggered: {
                    filePicker.open()
                }
            }
            MenuItem {
                height: 30
                text: qsTr("退出")
                onTriggered: {
                    Qt.quit()
                }
            }
        }
        Menu {
            id: editMenu
            width: 150
            title: qsTr("编辑")
        }
        Menu {
            id: confMenu
            width: 150
            title: "配置"
            MenuItem {
                height: 30
                text: qsTr("采集配置")
                onTriggered: {
                    deviceConfig.openDeviceConfig()
                }
            }
        }
    }

    header: Rectangle {
        height: 40
        Row {
            anchors.fill: parent
            spacing: 6
        }
    }


    FilePicker {
        id: filePicker
    }



    Item {
        Loader {
            id: pageLoader
            anchors.fill: parent
            anchors.margins: 10
            sourceComponent: dataReviewPage
        }
    }

    DeviceConfig {
        id: deviceConfig
    }


    Component {
        id: dataReviewPage
        DataReview {

        }
    }
    Component {
        id: streamPage
        Stream {}
    }

    footer: TabBar {}
}
