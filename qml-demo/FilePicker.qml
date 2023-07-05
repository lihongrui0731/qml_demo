import QtQuick 2.15
import QtQuick.Dialogs 1.3

FileDialog {
    id: fileDialog
    title: "请选择文件"
    folder: shortcuts.home
//    Component.onCompleted: console.log("file dialog completed")
    onAccepted: {
        console.log("you've chose:", fileDialog.fileUrl)
    }
    onRejected: {
        console.log("Canceled")
    }
}
