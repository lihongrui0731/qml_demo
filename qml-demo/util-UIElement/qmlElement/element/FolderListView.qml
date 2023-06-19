import QtQuick 2.15
import Qt.labs.folderlistmodel 2.12
import QtQuick.Controls 2.15
import FileManager 1.0
import HostManager 1.0

import "./../button"
import "../../resources/js/public.js" as Fun
import 'qrc:/js/config.js' as UI_config
import "qrc:/js/color.js" as UI_color

/**
QML目录导航列表
    /目录文件
    /文件过滤
    /递归
    /事件
    /高亮当前行
    /当前选项
    /目录切换动画
    /限制根目录+-
usage:
    FolderListView{
        onItemClicked:{
            console.debug(JSON.stringify(item));
        }
    }
**/
Item{
    id: fileManager
    // 点击事件（包括文件和目录）
    signal itemClicked(var item)
    property var files:[]
    property string current_dir: ""
    property string channelId: ""
    property real indx_: fileManager.channelId!=""?1:2;

    property bool doExport: false
    property bool canOpen: true
    property bool canExport: true
    property bool canDelete: true
    // 文件名显示长度

    property string pre:Qt.platform.os=="linux"?"file:":"file:///";

    signal openFile( string file );

    function reflash(){
        var temp = lv_rect.initFolder
        folderModel.folder = "";
        folderModel.folder = temp;
        lv.forceLayout();
    }

    onDoExportChanged: {
        if( fileManager.doExport ){
            var res = fm.exportFiles( JSON.stringify(fileManager.files) , deviceList.chosen )
            /*
            if( !res ){
                messageDialog.show("文件导出中断");
                // progress.visible = false
                deviceList.close();
            }else{
                messageDialog.show("文件导出完成");
                // progress.visible = false
                fm.openDir(deviceList.chosen);
            }
            */
        }
    }

    Connections{
        target: fm
        function onExportFinish(status) {
            // console.log( status )
            if( status === "success" ){
                messageDialog.show("文件导出完成");
                progress.visible = false
                if( root.sys != "linux" ){
                    fm.openDir(deviceList.chosen);
                }
            }else if( status === "error" ){
                messageDialog.show("文件导出中断");
                progress.visible = false
            }else if( status === "noPath" ){
                messageDialog.show("目标目录不存在");
                progress.visible = false
            }else{
                messageDialog.show("error");
                progress.visible = false
            }
            fileManager.doExport = false
        }
    }

    Loading{
        id: progress
        anchors.fill: parent
        //bgcolor: "transparent"
        visible: false
    }

    CheckBox{
        id: checkAll
        indicator.width: 28 / root.dpi
        indicator.height: 28 / root.dpi
        checked: false
        text: qsTr("全选")

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        x:1

        font.pixelSize: lv_rect.fontSize
        onCheckedChanged: {
            var len = lv.children[0].children.length
            // 0 是根目录 1 是上一页
            for( let i = 2; i < len; i++ ){
                if( lv.children[0].children[i].children[1] != undefined
                        && lv.children[0].children[i].children[1].objectName === "checkbox"
                        && lv.children[0].children[i].children[1].visible
                        ){
                   lv.children[0].children[i].children[1].checked = checkAll.checked
                   // console.log( lv.children[0].children[i].children[1].text )
                }
            }
        }
    }

    ActionButton{
        id: deleMore
        visible: fileManager.canDelete
        name: "批量删除"
        anchors.leftMargin: 20
        anchors.left: checkAll.right
        anchors.bottom: parent.bottom
        onActionClicked: {
            if( fileManager.files.length <= 0 ){
                messageDialog.show("请选择要删除的文件/文件夹")
                return
            }
            sureDelete.open()
        }
    }

    ActionButton{
        id: hostList
        name: "文件下载"
        visible: root.sys == "linux"
        anchors.right: open_btn.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 20/root.dpi
        w: 180/root.dpi
        onActionClicked: {
            var ip = hm.findHost();
            if( ip === "" ){
                messageDialog.show("请检查网线");
            }else{
                var tmp = ip.split(".");
                tmp.pop();
                tmp = tmp.join(".")+".x";
                messageDialog.show("连接网线后\n设置电脑ip为"+tmp+"\n访问http://"+ip+"/data");
            }
        }
    }

    ActionButton{
        id: open_btn
        name: "打开所在文件夹"
        visible: root.sys !== "linux"
        anchors.right: export_btn.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 20/root.dpi
        w: 180/root.dpi
        onActionClicked: {
            fm.openDir( lv_rect.currentPath )
        }
    }

    ActionButton{
        id: export_btn
        name: "导出"
        visible: fileManager.canExport
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onActionClicked: {
            if( fileManager.files.length <= 0 ){
                messageDialog.show("请选择要导出的文件/文件夹")
                return
            }

            var dist = fm.eDevice()
            if( dist.length <= 0 ){

                messageDialog.show("未发现外接存储设备");
                return false
            }

            device_model.clear()
            for( var i = 0; i < dist.length; i++ ){
                device_model.append({"text":dist[i]})
            }

            device_.currentIndex = 0
            deviceList.open()

            //var path = "h:";
            //fm.exportFiles( JSON.stringify(fileManager.files) , path )
        }
    }

    Dialog{
        id: sureDelete
        title: "删除确认"
        anchors.centerIn: parent
        implicitWidth: 300/root.dpi
        modal:true
        z:999
        standardButtons: Dialog.CloseOnEscape | Dialog.Yes | Dialog.No

        scale: root.checkRotation()? -1 :1
        transformOrigin: root.checkRotation()?Popup.TopLeft:Popup.Center

        Overlay.modal: Rectangle{
            color: "#33333333"
            rotation: page.rotation
            x:-page.x
            y:-page.y
        }

        Text{
            text: "确认要删除" + files.length + "个文件/文件夹吗"
            font.pixelSize: 16/root.dpi
            x:0
            y:0
        }

        onAccepted: {
            var res = fm.deleteFile1( JSON.stringify(fileManager.files) )
            if( !res ){
                messageDialog.show("文件删除失败");
            }else{
                fileManager.files = [];
            }
        }

        Component.onCompleted: {
            sureDelete.standardButton(Dialog.Yes).text = qsTr("是")
            sureDelete.standardButton(Dialog.No).text = qsTr("否")
        }

    }

    Rectangle{
        id: lv_rect
        width: parent.width
        height: parent.height - 50/root.dpi
        border.width: 1

        property int rowHeight: 30/root.dpi                 // 行高
        property int fontSize: 20/root.dpi                  // 字体大小
        property color hightlightColor: UI_color[UI_config[root.usedConfig].colorType].file_line_chosen     // 高亮行背景色
        property var fileFilter : ["*.jpg","*.tdms","*.rsd","*.csv"]           // 文件过滤器
        property string initFolder: fileManager.pre+fm.getRootPath(fileManager.channelId)  // 初始目录
        property string rootFolder : fileManager.pre+fm.getRootPath(fileManager.channelId)   //'../'            // 限制根目录，不可再往上查找
        property string currentPath : fm.getRootPath(fileManager.channelId)  //'../'            // 限制根目录，不可再往上查找
        property string relativePath : "./"   //'../'            // 限制根目录，不可再往上查找
        property int count: 0 // 文件个数

        Text {
            id: fileCount
            text: "文件数：" + lv_rect.count
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            font.pixelSize: lv_rect.fontSize
            anchors.margins: 5/root.dpi
        }

        Text {
            id: filePath
            text: "当前路径：" + lv_rect.relativePath
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            font.pixelSize: lv_rect.fontSize
            anchors.margins: 5/root.dpi
            width: parent.width * 0.7
            clip: true
        }

        // 文件管理模块
        Fm{
            id: fm
        }

        // 主机管理
        Hm{
            id: hm
        }

        Dialog{
            id: deviceList
            title: "导出到："
            anchors.centerIn: parent
            modal:true
            height: 200/root.dpi
            width: 250/ root.dpi
            standardButtons: Dialog.CloseOnEscape | Dialog.Yes | Dialog.No

            scale: root.checkRotation()? -1 :1
            transformOrigin: root.checkRotation()?Popup.TopLeft:Popup.Center

            property string chosen: ""

            Overlay.modal: Rectangle{
                color: "#33333333"
                rotation: page.rotation
                x:-page.x
                y:-page.y
            }

            Item{
                anchors.fill: parent
                ComboBoxItem {
                  id: device_
                  font.pixelSize: 16/root.dpi
                  anchors.topMargin: 20/root.dpi
                  width: 200/root.dpi
                  height: 50/root.dpi
                  model: ListModel {
                    id: device_model
                    ListElement { text: "1"; }
                  }
                  onAccepted: {
                    if (combo.find(currentText) === -1) {
                        model.append({text: editText})
                        currentIndex = combo.find(editText)
                    }
                  }
                  onCurrentTextChanged: {
                      deviceList.chosen = currentText
                  }
                }

            }

            onAccepted: {
                progress.visible = true
                console.log("文件导出")
                fileManager.doExport = true;
            }
            Component.onCompleted: {
                deviceList.standardButton(Dialog.Yes).text = qsTr("是")
                deviceList.standardButton(Dialog.No).text = qsTr("否")
            }

        }

        ListView {
            id: lv
            width: parent.width
            height: parent.height - 30/root.dpi
            clip: true
            onCountChanged: {
                lv.getFolderInfo()
            }
            //-----------------------------------------
            // public
            //-----------------------------------------

            //-----------------------------------------
            // 模型
            //-----------------------------------------
            model: folderModel

            FolderListModel {
                id: folderModel
                nameFilters: lv_rect.fileFilter
                folder: lv_rect.initFolder
                showDotAndDotDot: true
                showDirsFirst: true
                showDirs: true
                sortField: FolderListModel.Type
                rootFolder: lv_rect.rootFolder
                showHidden: true
                onFolderChanged:{
                    lv.getFolderInfo()
                }

            }

            //-----------------------------------------
            // 场景切换动画
            //-----------------------------------------
            // PropertyAnimation on x{id: aniForward; from: lv.width; to: 0}
            // PropertyAnimation on x{id: aniBack; from: -lv.width; to: 0}

            //-----------------------------------------
            // 高亮行
            //-----------------------------------------
            highlightFollowsCurrentItem: false
            highlight: Rectangle {
                width: lv.width-2/root.dpi;
                height: lv_rect.rowHeight
                x:1
                color: lv_rect.hightlightColor
                y: lv.currentItem?lv.currentItem.y:1
                Behavior on y { PropertyAnimation { properties: 'y'; duration: 300; easing.type: Easing.OutExpo } }
                //Behavior on y { SpringAnimation { spring: 2; damping: 0.1; duration:100 } }
            }

            //-----------------------------------------
            // 代理
            //-----------------------------------------
            delegate: Item{
                width: parent?parent.width:0
                height:lv_rect.rowHeight
                property bool showLabel: false

                CheckBox{
                    indicator.width: 28 / root.dpi
                    indicator.height: 28 / root.dpi
                    checked: false
                    text: fileName
                    visible: fileIsDir && fileName != "." && fileName != "..";
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 0
                    height: parent.height - 2/root.dpi
                    width: parent.width - 2/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    x: 1
                    z: 998
                    clip:true
                    background: Rectangle{
                        color: "transparent"
                    }

                    onCheckedChanged: {
                        const file = fileURL.toString().substring( fileManager.pre.length, fileURL.toString().length ) ;

                        if( this.checked ){
                            // 选中
                            parent.ListView.view.currentIndex = index
                            this.background.color = lv_rect.hightlightColor;
                            //parent.Text.background.color = lv_rect.hightlightColor;
                            fileManager.files = Fun.addDataToArr( fileManager.files , file )
                        }else{
                            // 不选中
                            this.background.color = "#ffffff";
                            //parent.Text.background.color =  "#ffffff";
                            fileManager.files = Fun.delDataFromArr( fileManager.files , file )
                        }
                    }

                }

                CheckBox{
                    objectName: "checkbox"
                    indicator.width: 28 / root.dpi
                    indicator.height: 28 / root.dpi
                    checked: false
                    text: fileName
                    visible: !fileIsDir;
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 0
                    font.pixelSize: lv_rect.fontSize
                    width: parent.width - ( rightPos() + 2)/root.dpi
                    height: parent.height - 2/root.dpi
                    x: 1
                    z: 998
                    clip:true
                    background: Rectangle{
                        color: "transparent"
                    }

                    function rightPos(){
                        var a = 0;
                        if( fileManager.canDelete && fileManager.canOpen ){
                            a = 200
                        }else if( fileManager.canDelete || fileManager.canOpen  ){
                            a = 120
                        }else{
                            a = 40
                        }
                        return a;
                    }

                    onCheckedChanged: {
                        const file = fileURL.toString().substring( fileManager.pre.length, fileURL.toString().length ) ;

                        if( this.checked ){
                            // 选中
                            parent.ListView.view.currentIndex = index
                            this.background.color = lv_rect.hightlightColor;
                            fileManager.files = Fun.addDataToArr( fileManager.files , file )
                        }else{
                            // 不选中
                            this.background.color = "#ffffff";
                            fileManager.files = Fun.delDataFromArr( fileManager.files , file )
                        }

                    }
                }

                Text {
                    objectName: ""
                    text: fileName === '.'?"根目录":(fileName === ".."?"上一页":"*")
                    visible: fileName === '.' || fileName === '..';
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: lv_rect.fontSize
                    x: 10/root.dpi
                }

                Text {
                    objectName: ">"
                    text: ">";
                    visible:fileIsDir;
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right;
                    anchors.rightMargin: 10/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    z: 999
                }


                Text {
                    objectName: "filename"
                    text: Fun.formatSize(fileSize);
                    visible:!fileIsDir;
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right;
                    anchors.rightMargin: getSizePosition()/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    z: 999

                    function getSizePosition(){
                        var pos = 0
                        if( fileManager.canDelete && fileManager.canOpen ){
                            pos = 140
                        }else if( fileManager.canDelete || fileManager.canOpen ){
                            pos = 70
                        }else{
                            pos = 10
                        }
                        return pos;
                    }

                }

                ActionButton{
                    name: "删除"
                    visible:!fileIsDir && fileManager.canDelete;
                    anchors.right: parent.right;
                    anchors.rightMargin: getDeletePosition()/root.dpi
                    anchors.verticalCenter: parent.verticalCenter
                    height: 25/root.dpi
                    width: 50/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    z: 1001
                    onActionClicked: {
                        sureDialog.open()
                    }
                    function getDeletePosition(){
                        var pos = 0
                        if( fileManager.canOpen ){
                            pos = 70
                        }else{
                            pos = 10
                        }
                        return pos;
                    }
                }

                ActionButton{
                    name: "打开"
                    visible:!fileIsDir && fileManager.canOpen;
                    anchors.right: parent.right;
                    anchors.rightMargin: 10/root.dpi
                    anchors.verticalCenter: parent.verticalCenter
                    height: 25/root.dpi
                    width: 50/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    z:1001
                    onActionClicked: {
                        var ext = fileURL.toString().substring(fileURL.toString().length - 4, fileURL.toString().length);
                        if( ext === '.jpg' ){
                            photoDialog.show( fileURL.toString() );
                        }else if( ext === ".csv" ){
                            messageDialog.show("请下载后打开");
                        }else{
                            const directFile = fileURL.toString().substring( fileManager.pre.length, fileURL.toString().length )
                            root.channels = [];
                            fileManager.openFile(directFile);
                        }
                     }
                }



                Rectangle{
                    width: parent.width
                    height:1
                    color: '#f0f0f0'
                    y: parent.height - 1
                    x: 0
                }

                Popup{
                    x: 30 / root.dpi
                    y: 30 / root.dpi
                    width: parent.width - 30 / root.dpi
                    height: parent.height * 2
                    visible: parent.showLabel
                    z: 1002

                    scale: root.checkRotation()? -1 :1
                    transformOrigin: root.checkRotation()?Popup.TopLeft:Popup.Center

                    Text{
                        width: parent.width - 20 / root.dpi
                        text: fileName
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: lv_rect.fontSize
                        anchors.verticalCenter: parent.verticalCenter
                        x: 10 / root.dpi
                    }

                }

                MouseArea{
                    anchors.fill:  parent
                    z:1000
                    cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                    hoverEnabled: true
                    onEntered: {
                        if( !fileIsDir )
                        parent.showLabel = true
                    }
                    onExited: {
                        parent.showLabel = false
                    }

                    onClicked: {
                       if( fileIsDir ){
                           parent.children[0].checked = !parent.children[0].checked
                       }else{
                           parent.children[1].checked = !parent.children[1].checked
                       }
                    }

                    onDoubleClicked: {
                        parent.ListView.view.currentIndex = index

                        // 触发节点点击事件
                        var json = {
                            isDir: fileIsDir,
                            name: fileName,
                            path: filePath,
                            url: fileURL.toString(),
                            baseName: fileBaseName,
                            suffix: fileSuffix,
                            size: fileSize,
                            modified: fileModified,
                            accessed: fileAccessed,
                            folderUrl: getFolder(fileURL.toString())
                        };
                        // console.debug(JSON.stringify(json));
                        fileManager.itemClicked(json);

                        // 目录处理
                        if (fileIsDir){
                            // 限制根目录
                            if (lv_rect.rootFolder != ''){
                                var folderURL = (fileURL + '/').toLowerCase();
                                var rootFolderURL = Qt.resolvedUrl(lv_rect.rootFolder).toLowerCase();
                                if (rootFolderURL.indexOf(folderURL) != -1){
                                    //console.log('limit root: ' + rootFolderURL);
                                    return;
                                }
                            }
                            changeFolder(json.name, json.url);
                        }
                    }

                    // 获取文件所在的目录
                    function getFolder(filePath)
                    {
                        var n = filePath.lastIndexOf('/');
                        var folder_ = filePath.substr(0, n+1);
                        return folder_;
                    }

                    // 更改目录（修改model并启动相应的动画）
                    function changeFolder(folderName, folderURL){
                        //folderModel.showDotAndDotDot = folderURL!=rootFolderURL;  // （无效）
                        if( folderName === '.' ){
                            folderModel.folder = lv_rect.rootFolder;
                        }else{
                            folderModel.folder = folderURL;
                        }
                        if( folderName == ".." )
                            lv_rect.relativePath = "./"
                        else
                            lv_rect.relativePath = folderName + "/"

                        /*  动画
                            if (folderName == '..'  ) aniBack.start();
                            else if (folderName == '.' ) {}
                            else aniForward.start();
                        */
                        fileManager.files = []
                    }

                }

                Dialog{
                    id: sureDialog
                    title: "删除确认"
                    anchors.centerIn: parent
                    implicitWidth: 230/root.dpi
                    implicitHeight: 100/root.dpi
                    modal:true
                    z:999
                    scale: root.checkRotation()? -1 :1
                    transformOrigin: root.checkRotation()?Popup.TopLeft:Popup.Center

                    standardButtons: Dialog.CloseOnEscape | Dialog.Yes | Dialog.No

                    Overlay.modal: Rectangle{
                        color: "#33333333"
                        rotation: page.rotation
                        x:-page.x
                        y:-page.y
                    }

                    onAccepted: {
                        deleteFile()
                    }

                    Component.onCompleted: {
                        sureDialog.standardButton(Dialog.Yes).text = qsTr("是")
                        sureDialog.standardButton(Dialog.No).text = qsTr("否")
                    }
                }

                function deleteFile(){
                    // console.log(fileURL.toString().substring( fileManager.pre.length,  fileURL.toString().length ) )
                    // 删除文件事件
                    // Qt.dir
                    var res = fm.deleteFile1( JSON.stringify( [fileURL.toString().substring( fileManager.pre.length, fileURL.toString().length )] ))
                    if( !res ){

                        messageDialog.show("文件删除失败");
                        sureDialog.close();
                    }
                }
            }

            //获取文件夹里的文件数量
            function getFolderInfo(){
                if( folderModel.folder.toString().toLowerCase() == lv_rect.rootFolder.toLowerCase() || folderModel.folder.toString() == lv_rect.rootFolder.toLowerCase()+"/." || folderModel.folder.toString().search( new RegExp(lv_rect.rootFolder+"\/[^\/]+\/[^a-zA-Z0-9]+","i") ) == 0 ){
                    lv_rect.count = folderModel.count - 1
                }else{
                    lv_rect.count = folderModel.count - 2
                }
                lv_rect.currentPath = folderModel.folder.toString().substring( fileManager.pre.length,  folderModel.folder.toString().length )
                // console.log( lv_rect.currentPath + "aaaaaa" )
            }
        }

    }
}
