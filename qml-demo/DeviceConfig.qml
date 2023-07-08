import QtQuick 2.15
import QtQuick.Controls 2.15
//import QtQuick.Dialogs 1.2

Dialog {
    id: deviceConfigDialog
    title: "采集配置"
    height: root.height * 0.7
    width: root.width * 0.4
    anchors.centerIn: parent
    standardButtons: Dialog.Ok | Dialog.Cancel | Dialog.Apply

    property var uploadList: []
    property var confirmedUploadList: []

    function openDeviceConfig() {
        console.log("device config dialog opening.......")
        deviceConfigDialog.setUploadList(JSON.stringify(root.uploadList))
        console.log(JSON.stringify(this.uploadList))
//        uploadListRepeater.model = deviceConfigDialog.uploadListTemp
        deviceConfigDialog.open()
    }
    function setUploadList(info) {
        deviceConfigDialog.uploadList = JSON.parse(info)
        deviceConfigDialog.confirmedUploadList = []
    }
    function changeUploadListTemp(key, value) {
        for (var i=0; i<deviceConfigDialog.uploadListTemp.length; i++) {
//            console.log(deviceConfigDialog.uploadList[i][key])
            if(deviceConfigDialog.uploadListTemp[i][key] !== undefined) {
                deviceConfigDialog.uploadListTemp[i][key] = value
            }
        }
    }
    function changeUploadList(index, key, value) {
        if(deviceConfigDialog.uploadList[index][key] !== undefined) {
            deviceConfigDialog.uploadList[index][key] = value
        }
    }
    function confirmUploadList() {
        deviceConfigDialog.confirmedUploadList = deviceConfigDialog.uploadList
        root.changeUploadList(deviceConfigDialog.confirmedUploadList)
        console.log("confirmed!!!!")
//        console.log(JSON.stringify(deviceConfigDialog.uploadList))
//        root.dispatchUploadList(deviceConfigDialog.uploadList)
    }

    function setDeviceParams(params) {
        var result = {}
    }

    Component.onCompleted: {
//        setUploadList(root.uploadList)
        setDeviceParams(root.deviceParams)
    }
    onAccepted: {
        console.log("saved!")
        confirmUploadList()
    }
    onRejected: {
        console.log("canceled!")
        console.log(JSON.stringify(deviceConfigDialog.uploadList))
        console.log(JSON.stringify(deviceConfigDialog.confirmedUploadList))
        console.log(JSON.stringify(root.uploadList))
    }
    onApplied: {
        console.log('applied!')
        confirmUploadList()
    }

    Column {
        spacing: 2
        Repeater {
            id: uploadListRepeater
            model: deviceConfigDialog.uploadList.length
            CheckBox {
                property var key: Object.keys(deviceConfigDialog.uploadList[index])[0]
                checked: deviceConfigDialog.uploadList[index][key]
                text: key
                onCheckedChanged: {
                    changeUploadList(index, key, checked)
                    console.log(JSON.stringify(deviceConfigDialog.uploadList))
                    console.log(JSON.stringify(deviceConfigDialog.confirmedUploadList))
                }
            }
        }
    }

}

