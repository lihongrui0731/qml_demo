import QtQuick 2.15
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

ListView {
    id:listView
    width: parent.width
    height: parent.height

    property bool mRadioBtnVisible: false
    FolderListModel {
        id: folderModel
        showDotAndDotDot: true
        showDirs: true
        folder: /*"file:///E:"*/ fileSystemModel.mySetRootPath()
        nameFilters: [".dat"]

    }

    Component {
        id: fileDelegate
        Row{
            Rectangle{
                color: "#FFDEAD"
                id:mRadioBtn
                width: 40
                height: 40
                border.color: "#ffffff"
                border.width: 1
                RadioItem{
                    id:mRadioBtn1

                    //anchors.fill: parent
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: mRadioBtnVisible

                    onCheckedChanged: {
                        if(mBtnText.text == ".."){

                        }
                        else if(mBtnText.text == "."){}
                        else{
                            fileSystemModel.addRemoveFile(checked,folderModel.folder.toString(),(folderModel.folder+"/"+mBtnText.text).toString());
                        }

                    }
                }
                MouseArea{
                    anchors.fill: parent
                    cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                    onClicked: {
                        mRadioBtn1.checked = !mRadioBtn1.checked
                    }
                }
            }
            Rectangle{
                height: 40
                width:listView.width-mRadioBtn.width
//                color: "#FFDEAD"
                border.color: "#ffffff"
                border.width: 1
                color: rect_mouse.pressed ? "#01968c" : (rect_mouse.containsMouse ? "#20c9b3" : "#FFDEAD")

                Text {
                    id:mBtnText
    //                width: parent.width-mRadioBtn.width-misFolder.width
                    anchors.verticalCenter: parent.verticalCenter
                    text: fileName
                }

                Text {
                    id:misFolder
                    width: 20
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    Component.onCompleted:  {
                        var mIndex = folderModel.indexOf(folderModel.folder+"/"+mBtnText.text)
                        if(mIndex !== -1 && folderModel.isFolder(mIndex)){
                            misFolder.text= ">  "
                        }
                    }
                }

                MouseArea{
                    anchors.fill: parent
                    id:rect_mouse
                    hoverEnabled: true //是否处理悬浮事件，默认false，只有按下鼠标键时才处理鼠标事件，为true时即使没有按下鼠标键也会作相应的处理
                    preventStealing:true//默认为false，为true时可以防止当前鼠标事件被其它对象拦截。
                    propagateComposedEvents:true//默认为 false，当设置为 true 时，就可以将事件传递给重叠的其他鼠标区域了
                    cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                    enabled: true
                    onClicked: {
                        if(mBtnText.text == ".."){
                            var mFolder = folderModel.folder.toString()
                            var newFolder = mFolder.slice(0,mFolder.lastIndexOf("/",100000))
                            folderModel.folder = newFolder

                        }
                        else if(mBtnText.text == "."){}
                        else{
                            var mIndex = folderModel.indexOf(folderModel.folder+"/"+mBtnText.text)
                            if(mIndex !== -1 && folderModel.isFolder(mIndex)){
                                folderModel.folder = folderModel.folder+"/"+mBtnText.text
                            }
                        }
                    }
                    onPressAndHold: {
                        mRadioBtnVisible = true
                    }
                }
            }

        }

    }
    model: folderModel
//    header:
//    Row{
//        height: 40
//        width: listView.width
//        Rectangle{
//            height: parent.height
//            width: 120
//            Text {
//                anchors.verticalCenter: parent.verticalCenter
//                text: "新建文件夹"
//            }
//        }
//        Rectangle{
//            height: parent.height
//            width: 120

//            Text {
//                anchors.verticalCenter: parent.verticalCenter
//                text: "新建文件"
//            }
//        }
//    }
    delegate: fileDelegate
    footer: Rectangle{
        id:mFooterRec
        height: 40
        width: 150
        anchors.horizontalCenter: parent.horizontalCenter

        Row{
            spacing: 5
            Rectangle{
                width:60
                height: 40
//                color: "#6C7B8B"
                radius: 4
                color: rect_mouse2.pressed ? "#01968c" : (rect_mouse2.containsMouse ? "#20c9b3" : "#6C7B8B")

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "删除"
                }
                MouseArea{
                    id:rect_mouse2
                    anchors.fill: parent
                    hoverEnabled: true //是否处理悬浮事件，默认false，只有按下鼠标键时才处理鼠标事件，为true时即使没有按下鼠标键也会作相应的处理
                    preventStealing:true//默认为false，为true时可以防止当前鼠标事件被其它对象拦截。
                    propagateComposedEvents:true//默认为 false，当设置为 true 时，就可以将事件传递给重叠的其他鼠标区域了
                    cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                    enabled: true
                    onClicked: {
                        fileSystemModel.removeAll()
                        mRadioBtnVisible = false
                    }
                }
            }
            Rectangle{
                width:60
                height: 40
//                color: "#6C7B8B"
                radius: 4
                color: rect_mouse1.pressed ? "#01968c" : (rect_mouse1.containsMouse ? "#20c9b3" : "#6C7B8B")

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: "退出"
                }
                MouseArea{
                    id:rect_mouse1
                    anchors.fill: parent
                    hoverEnabled: true //是否处理悬浮事件，默认false，只有按下鼠标键时才处理鼠标事件，为true时即使没有按下鼠标键也会作相应的处理
                    preventStealing:true//默认为false，为true时可以防止当前鼠标事件被其它对象拦截。
                    propagateComposedEvents:true//默认为 false，当设置为 true 时，就可以将事件传递给重叠的其他鼠标区域了
                    cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                    enabled: true
                    onClicked: {
                        fileSystemModel.closeWindow()
                    }
                }
            }
        }

    }
}

