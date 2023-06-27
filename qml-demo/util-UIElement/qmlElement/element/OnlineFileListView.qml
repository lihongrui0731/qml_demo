import QtQuick 2.15
import QtQuick.Controls 2.15
import FileManager 1.0

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
    property var choosen: []
    property string rootUrl: ""
    property string url: ""

    signal download( var dir , var files );

    function getNowDate(){
        var currentTime = new Date();
        var timeString = currentTime.toLocaleTimeString(Qt.locale(), Locale.LongFormat);
        return timeString;
    }

    function progress( fileName , progress_ ){
        var offset = 2;
        if( lv_rect.rootFolder == lv_rect.currentPath ){
            offset = 1;
        }

        for( let i = 0; i < fileManager.files.length; i++  ){
            if( fileName == fileManager.files[i].name ){
                fileManager.files[i].per = progress_;
                folderModel.set(i+offset, fileManager.files[i])
            }
        }
    }

    function finishDownload( fileName ){

        var now = getNowDate();
        console.log( fileName + "downFinish ---- "+ now );

        var offset = 2;
        if( lv_rect.rootFolder == lv_rect.currentPath ){
            offset = 1;
        }

        for( let i = 0; i < fileManager.files.length; i++  ){
            if( fileName == fileManager.files[i].name ){
                fileManager.files[i].isDownload = false;
                fileManager.files[i].downloadfinish = true;
                folderModel.set(i+offset, fileManager.files[i])
            }
        }

    }

    function downloadFiles( dir , files_ ){

        var now = getNowDate();
        console.log( "downStart ---- "+ now );

        var offset = 2;
        if( lv_rect.rootFolder == lv_rect.currentPath ){
            offset = 1;
        }

        for( let i = 0; i < fileManager.files.length; i++  ){
            if( files_.indexOf(fileManager.files[i].name) != -1 ){
                fileManager.files[i].isDownload = true;
                fileManager.files[i].downloadfinish = false;
                folderModel.set(i+offset, fileManager.files[i])
            }
        }

        fileManager.download(dir , files_);
    }

    function changeUrL( url_ ){
        fileManager.url = url_;
        lv_rect.currentPath = url_;

        console.log( lv_rect.rootFolder );
    }

    function setFileList(){
        folderModel.clear()
        folderModel.append({ "name":".", "type":"directory", "mtime":"" , "size" : 0 , "isDownload":false , "downloadfinish":false , "per":"0"})

        if( lv_rect.rootFolder != lv_rect.currentPath) {
            folderModel.append({ "name":"..", "type":"directory", "mtime":"" , "size" : 0, "isDownload":false , "downloadfinish":false , "per": "0"})
        }

        for( let i = 0; i < fileManager.files.length; i++ ){
            var tmp = fileManager.files[i];
            if( !tmp.hasOwnProperty("size") ){
                 tmp["size"] = 0;
            }
            if( !tmp.hasOwnProperty("isDownload") ){
                 tmp["isDownload"] = false;
            }
            if( !tmp.hasOwnProperty("downloadfinish") ){
                 tmp["downloadfinish"] = false;
            }
            if( !tmp.hasOwnProperty("per") ){
                 tmp["per"] = "0";
            }

            folderModel.append( tmp )
        }
        lv_rect.count = fileManager.files.length;
    }

    Connections{
        target: fm
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

            let i = lv_rect.rootFolder == lv_rect.currentPath?1:2

            for( i ; i < len; i++ ){
                if( lv.children[0].children[i].children[1] != undefined
                        && lv.children[0].children[i].children[1].objectName === "checkbox"
                        && lv.children[0].children[i].children[1].visible
                        ){
                   lv.children[0].children[i].children[1].checked = checkAll.checked
                }
            }
        }
    }

    ActionButton{
        id: openFolderBtn
        name: "打开设备文件夹"
        visible: root.sys !== "linux"
        anchors.right: downloadBtn.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 20/root.dpi
        w: 180/root.dpi
        onActionClicked: {
            console.log("设备路径：" + lv_rect.currentPath );
            fm.openSharedDir( lv_rect.currentPath )
        }
    }

    ActionButton{
        id: downloadBtn
        name: "下载"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onActionClicked: {
            if( fileManager.choosen.length <= 0 ){
                messageDialog.show("请选择要下载的文件")
                return
            }
            fileManager.downloadFiles( lv_rect.currentPath , fileManager.choosen )
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
        property var fileFilter : ["*.jpg","*.tdms"]           // 文件过滤器
        property string rootFolder: fileManager.rootUrl
        property string currentPath : fileManager.url   //'../'            // 限制根目录，不可再往上查找
        property int count: 0 // 文件个数

        Text {
            id: fileCount
            text: "文件数：" + lv_rect.count
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            font.pixelSize: lv_rect.fontSize
            anchors.margins: 5/root.dpi
        }

        ListView {
            id: lv
            width: parent.width
            height: parent.height - 30/root.dpi
            clip: true
            onCountChanged: {

            }
            //-----------------------------------------
            // public
            //-----------------------------------------

            //-----------------------------------------
            // 模型
            //-----------------------------------------
            model: folderModel

            ListModel {
                id: folderModel

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
                id: item_
                width: parent?parent.width:0
                height:lv_rect.rowHeight
                property string fileName: name
                Text{
                    text: name
                    visible: type =="directory" && name != "." && name != "..";
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height - 2/root.dpi
                    width: parent.width - 180/root.dpi - 2/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    x: 10 / root.dpi
                    z: 998
                }

                CheckBox{
                    objectName: "checkbox"
                    indicator.width: 28 / root.dpi
                    indicator.height: 28 / root.dpi
                    checked: false
                    text: name
                    visible: type != "directory";
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: lv_rect.fontSize
                    width: parent.width - 180/root.dpi - 2/root.dpi
                    height: parent.height - 2/root.dpi
                    x: 1
                    z: 998
                    background: Rectangle{
                        color: "#ffffff"
                    }
                    onCheckedChanged: {
                        const file = name;
                        if( this.checked ){
                            // 选中
                            parent.ListView.view.currentIndex = index
                            this.background.color = lv_rect.hightlightColor;
                            fileManager.choosen = Fun.addDataToArr( fileManager.choosen , file )
                        }else{
                            // 不选中
                            this.background.color = "#ffffff";
                            fileManager.choosen = Fun.delDataFromArr( fileManager.choosen , file )
                        }

                    }
                }

                Text {
                    objectName: ""
                    text: name === '.'?"根目录":(name === ".."?"上一页":"*")
                    visible: name === '.' || name === '..';
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: lv_rect.fontSize
                    x: 10/root.dpi
                }

                Text {
                    objectName: ">"
                    text: ">";
                    visible: type == "directory";
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right;
                    anchors.rightMargin: 10/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    z: 999
                }

                Text {
                    objectName: "fileSize"
                    text: Fun.formatSize( Number(size) );
                    visible: type != "directory";
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right;
                    anchors.rightMargin: 120/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    z: 999
                }

                ActionButton{
                    name: downloadfinish? "下载完成":(isDownload? String(per)+"%" :"下载" )
                    visible: type != "directory"
                    anchors.right: parent.right;
                    anchors.rightMargin: 10/root.dpi
                    anchors.verticalCenter: parent.verticalCenter
                    height: 25/root.dpi
                    width: 100/root.dpi
                    font.pixelSize: lv_rect.fontSize
                    z:1001
                    onActionClicked: {
                        var file = item_.fileName;
                        fileManager.downloadFiles( lv_rect.currentPath , [file] )
                    }
                }

                Rectangle{
                    width: parent.width
                    height:1
                    color: '#f0f0f0'
                    y: parent.height - 1
                    x: 0
                }

                MouseArea{
                    anchors.fill:  parent
                    z:1000
                    cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                    onDoubleClicked: {
                        parent.ListView.view.currentIndex = index

                        // 触发节点点击事件
                        var json = {
                            isDir: type == "directory",
                            name: name,
                            size: size,
                        };
                        // console.debug(JSON.stringify(json));
                        fileManager.itemClicked(json);

                        // 目录处理
                        if (type == "directory"){
                            // 限制根目录
                            changeFolder(json.name);
                        }
                    }

                    // 更改目录（修改model并启动相应的动画）
                    function changeFolder(folderName){
                        //folderModel.showDotAndDotDot = folderURL!=rootFolderURL;  // （无效）
                        if( folderName == "." ){
                            fileManager.changeUrL( lv_rect.rootFolder );
                        }else if( folderName == ".." ){
                            var n = lv_rect.currentPath.lastIndexOf('/');
                            var folder_ = lv_rect.currentPath.substr(0, n);
                            fileManager.changeUrL(folder_);
                        }else{
                           fileManager.changeUrL( lv_rect.currentPath + "/" + folderName )
                        }

                        fileManager.choosen = []
                    }

                    onClicked: {
                       if( type != "directory" ){
                           parent.children[1].checked = !parent.children[1].checked
                       }
                    }

                }
            }

            //获取文件夹里的文件数量
            function getFolderInfo(){
            }
        }

    }
}
