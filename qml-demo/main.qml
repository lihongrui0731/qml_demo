import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Controls.Material 2.12

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

    menuBar: MenuBar {
        Menu {
            id: fileMenu
            width: 100
            title: "文件"

            MenuItem {
                height: 30
                text: "新建采集"
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
            title: qsTr("编辑")
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
            anchors.fill: parent
            anchors.margins: 10
            sourceComponent: dataReviewPage
        }
    }


    Component {
        id: dataReviewPage
        DataReview {

        }
    }

    footer: TabBar {}
}
