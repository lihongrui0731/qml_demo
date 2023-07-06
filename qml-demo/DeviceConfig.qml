import QtQuick 2.15
import QtQuick.Controls 2.15
//import QtQuick.Dialogs 1.2

Dialog {
    title: "采集配置"
    height: root.height * 0.7
    width: root.width * 0.4
    anchors.centerIn: parent
    standardButtons: Dialog.Ok | Dialog.Cancel | Dialog.Apply

    property var uploadList: []

    property var uploadListTemp: []
    function openDeviceConfig() {
        console.log("device config dialog opening.......")
        deviceConfigDialog.setUploadList(root.uploadList)
        uploadListRepeater.model = deviceConfigDialog.uploadList
        deviceConfigDialog.open()
    }
    function setUploadList(info) {
        deviceConfigDialog.uploadList = info
    }
    function changeUploadListTemp(key, value) {
        for (var i=0; i<deviceConfigDialog.uploadListTemp.length; i++) {
//            console.log(deviceConfigDialog.uploadList[i][key])
            if(deviceConfigDialog.uploadListTemp[i][key] !== undefined) {
                deviceConfigDialog.uploadListTemp[i][key] = value
            }
        }
    }
    function changeUploadList(key, value) {
        for (var i=0; i<deviceConfigDialog.uploadList.length; i++) {
//            console.log(deviceConfigDialog.uploadList[i][key])
            if(deviceConfigDialog.uploadList[i][key] !== undefined) {
                deviceConfigDialog.uploadList[i][key] = value
            }
        }
    }
    function confirmUploadList() {
        root.changeUploadList(deviceConfigDialog.uploadList)
        console.log("confirmed!!!!")
        console.log(JSON.stringify(deviceConfigDialog.uploadList))
        root.dispatchUploadList(deviceConfigDialog.uploadList)
    }

    function setDeviceParams(params) {
        var result = {}
    }

    Component.onCompleted: {
        setUploadList(root.uploadList)
        setDeviceParams(root.deviceParams)
    }
    onAccepted: {
        console.log("saved!")
        confirmUploadList()
    }
    onRejected: {
        console.log("canceled!")
    }
    onApplied: {
        console.log('applied!')
        confirmUploadList()
    }

    Column {
        spacing: 2
        Repeater {
            id: uploadListRepeater
            model: deviceConfigDialog.uploadList
            CheckBox {
                property var key: Object.keys(modelData)[0]
                checkState: modelData[key]? Qt.Checked : Qt.Unchecked
                text: key
                onCheckStateChanged: {
//                    console.log("checkbox changed", key, checkState)
//                    console.log(modelData[key])
                    changeUploadList(key, checkState === 0? false : true)
                }
            }
        }
    }

}

