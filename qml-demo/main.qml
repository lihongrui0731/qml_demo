import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Controls.Material 2.12

import FileManager 1.0
import "./pages"
import "./config/deviceConfig.js" as Device_config
import 'qrc:/js/config.js' as UI_config
import 'qrc:/js/color.js' as UI_color

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

    property string usedConfig: appConfig;
    property bool showMouse: UI_config[root.usedConfig].showMouse

    property var colorConfig: UI_color[UI_color["used"]]
    property string cardColor: colorConfig["cardColor"]

    property var deviceConfig: Device_config.config
    property var deviceParams: root.getDeviceParams()
    property var uploadList: root.getUploadList()
    property string currentDate: Qt.formatDateTime(new Date(), "yyyy-MM-dd HH:mm:ss")

    function getDeviceParams() {
        return root.deviceConfig.params
    }
    function getUploadList() {
        var info = root.deviceConfig.uploadList
        var result = []
        var keys = Object.keys(info)
        for (var i=0; i<keys.length; i++) {
            var item = {}
            item[keys[i]] = info[keys[i]]
            result.push(item)
        }
        return result
    }
    function changeUploadList(info) {
        root.uploadList = info
    }

    function syncDeviceTime() {
        deviceConfig.params.timeSync = currentDate
    }

    Component.onCompleted: {
        syncDeviceTime()
    }

    menuBar: MenuBar {
        Menu {
            id: fileMenu
            width: 150
            title: "文件"

            MenuItem {
                height: 30
                text: "新建项目"
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
                    deviceConfigDialog.openDeviceConfig()
                }
            }
        }
    }

    header: Rectangle {
        height: 30
        Row {
            anchors.fill: parent
            spacing: 6
        }
    }


    DeviceConfig {
        id: deviceConfigDialog
    }

    FilePicker {
        id: filePicker
    }



    Item {
        width: root.width
        height: root.height - root.header.height - root.footer.height - root.menuBar.height
        Loader {
            id: pageLoader
            anchors.fill: parent
            anchors.margins: 5
//            width: parent.width
//            height: parent.height
            sourceComponent: streamPage
        }
    }

    Component {
        id: dataReviewPage
        DataReview {
            id: dataReview
            anchors.fill: parent
            width: parent.width
            height: parent.height
        }
    }
    Component {
        id: streamPage
        Stream {
            id: stream
            anchors.fill: parent
            width: parent.width
            height: parent.height
        }
    }

    Connections {

    }

    footer: TabBar {}
}
