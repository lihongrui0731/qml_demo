import QtQuick 2.15
import Qt.labs.platform 1.0
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQml.Models 2.15
import CLineChart 1.0

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config
import '../../resources/js/public.js' as JS

Item {
    id: spchart
    anchors.fill: parent
    clip: true;

    property string channelId: "sound";
    property string type: "rms"
    property string unity: ""
    property bool adding: false
    property var evType: []
    property real frameCount: 0;
    property real linePos: 0;
    property int currentIndex:0;
    property real fontSize: 10/root.dpi

    property real ymin: 0
    property real ymax: 1
    property string sourseType: "sound"

    property real dt: 0
    property int seconds: 10
    property real xmin: -spchart.seconds
    property real xmax: 0
    property real unitLength: spchart.seconds / spchart.dt
    property real unit: (spchart.xmax - spchart.xmin) / spchart.unitLength

    property var maxArr:[];
    property var dataArr:[]
    property int times: 0;

    property string lastTime:"" //上一次上报数据取值时间
    property bool reprocess: false
    property bool disabled: false
    property bool unique: false
    property bool isStop: false
    property bool isChangeSize: false
    signal reprocessFinish;

    Connections {
        target: root
        function onActiveChanged(){
            if( root.active ){
                chart.reInitial();
                chart.reSize();
            }
        }
    }

    function setFrameCount( channelId_ , count , time ){
        if( channelId_ === spchart.channelId ){
            spchart.frameCount = count
            spchart.seconds = Math.round(time / 1000);
            chart.reInitial();
        }
    }

    function setChannel( channelId_ ){
        if( spchart.disabled ) return;

        if( 1 || spchart.channelId !== channelId_ ){
            spchart.channelId = channelId_
            if( dataContent.file != "" ){

                chart.clearData();
                spchart.maxArr = [];
                spchart.dataArr = [];

                dm.getEigenvaluesByType( spchart.channelId , spchart.type );
            }else{
                dm.getEigenvalueData( spchart.channelId , spchart.type )
            }
        }
    }

    // 实时切换特征值
    function setType( value , unity ){
        if( spchart.unique ) return;
        if( spchart.disabled ) return;
        if( spchart.type !== value ){
            spchart.type = value
            spchart.unity = unity
            chart.clearData();
            spchart.maxArr = [];
            spchart.dataArr = [];
            dm.getEigenvalueData( spchart.channelId , spchart.type )
        }
        // 可能要清空数据 可能要读取历史数据
    }

    function getDataFromOpenData(){
        if( spchart.disabled ) return;
        chart.clearData();
        spchart.maxArr = [];
        spchart.dataArr = [];

        spchart.reprocess = true;
        dm.getEigenvaluesByType( spchart.channelId , spchart.type );
    }

    // 打开保存文件 切换特征值
    function changeType( type , unity ){

        if( spchart.disabled ) return;
        if( spchart.type !== type ){
            spchart.type = type
            spchart.unity = unity
            chart.clearData();
            spchart.dataArr = [];
            spchart.maxArr = [];
            dm.getEigenvaluesByType( spchart.channelId , spchart.type );
        }
    }

    function doAddData( data ){
        if( spchart.disabled ) return;
        if( data === "{}" ) return;

        var data_ = JSON.parse(data);
        if( !data_.dt ){
            return;
        }
        if(( data_.dt !== spchart.dt && data_["dt"] > 0 ) || spchart.times == 0 ){
            // 频率发生改变
            spchart.dt = data_.dt
            chart.df = spchart.unit;
            chart.rePaint();
        }

        if( spchart.type == "Level_A" ){
            return;
        }else{
            spchart.addXYData( data_["values"][spchart.type] );
            var data__ = { max:spchart.ymax , values: spchart.dataArr };
            chart.addJsonBatchData( data__ );
        }
    }

    function changeTimeLine( timeline , currentFrame ){
        if( spchart.disabled ) return;

        var currentIndex_ = currentFrame / spchart.frameCount * spchart.unitLength;
        if( currentIndex_ != spchart.currentIndex ){
            spchart.currentIndex = currentIndex_;
        }

        chart.setCurrentPos( currentIndex_ );

    }

    Connections {
        target: dm

        function onReprocessEv(){
            if( spchart.disabled ) return;
            spchart.reprocess = true;
        }

        function onReprocessEvFinish(){

            if( spchart.disabled ) return;
            spchart.reprocess = true;
            dm.getEigenvaluesByType( spchart.channelId , spchart.type );
        }

        function onSendEigenvaluesJson (s , channelId_ , type){
            // 切换历史通道数据

            if( spchart.disabled ) return;

            if( !s || type !== spchart.type ){
                return;
            }

            if( spchart.channelId === channelId_ ){

                spchart.dataArr = [];
                spchart.maxArr = [];
                var data_ = JSON.parse(s);

                if( spchart.isStop && spchart.sourseType != "sound" ){
                    var endTime = new Date( data_[data_.length-1]["baseTime"] ).getTime();
                    var startTime = new Date( data_[0]["baseTime"] ).getTime();
                    spchart.seconds = Math.ceil(( endTime - startTime ) / 1000 )
                }

                spchart.adding = true
                for( var i = 0; i < data_.length; i++ ){
                    spchart.doAddData( JSON.stringify(data_[i]) );
                }
                spchart.adding = false;
            }
        }

        function onEigenvalueChanged (channelId_ , s){
            if( spchart.type == "Level_A" || (spchart.sourseType != "sound" && root.isSound) ){
                return;
            }

            if( spchart.disabled ) return;
            // 实时添加特征值

            if( !s || spchart.adding ){
                return;
            }
            if( spchart.channelId === channelId_ ){
                var data_ = JSON.parse(s);
                if( spchart.lastTime === data_.baseTime ){
                    return;
                }
                spchart.lastTime = data_.baseTime;
                spchart.doAddData( s );
            }
        }

        function onSendEigenvalueByType( channelId_ , type_ , ev_ , dateTime , dt , frameCount ){
            if( type_ == "Level_A" || spchart.disabled ) return;
            //打开文件切换特征值
            if( channelId_ === spchart.channelId && type_ === spchart.type ){

                ev_ = JSON.parse(ev_);
                var ev__ = ev_[type_];

                spchart.seconds = dt * ev__.length;
                spchart.xmin = spchart.xmax - spchart.seconds
                spchart.unit = dt;
                spchart.unitLength = spchart.seconds / spchart.unit + 1;
                spchart.dt = dt
                chart.df = spchart.unit;

                if( ev__.length > 0 ){
                    spchart.addXYData( ev__ );
                    var data__ = { max:spchart.ymax , values: spchart.dataArr };
                    chart.addJsonBatchData( data__ );

                }else{
                    spchart.dataArr = [];
                    chart.clearData();
                }

                if( spchart.reprocess == true ){
                    spchart.reprocessFinish();
                }

            }
        }

        function onSendFinalEV( channelId , s , timeLen ){;

            if( spchart.disabled ) return;
            if( channelId !== spchart.channelId ){
                return;
            }

            spchart.dataArr = [];
            spchart.maxArr = [];
            chart.clearData();

            dm.getEigenvalueData( spchart.channelId , spchart.type )

        }
    }

    Rectangle{
        anchors.fill: parent
        color: "#CCCCCC"
        z:3
        visible: spchart.disabled

        Text{
            text: qsTr("特征值未开通")
            anchors.centerIn: parent
            font.pixelSize: 20 / root.dpi
        }
    }

    RhythmLineChart{
        id: chart
        anchors.fill: parent
        xAxisMax: spchart.xmax
        xAxisMin: spchart.xmin
        yAxisMax: spchart.ymax
        yAxisMin: 0
        xAxisTitle: qsTr("时间（s）");
        yAxisTitle: spchart.unity
        dpi: root.dpi
        fontSize: spchart.fontSize
        frameLen: spchart.unitLength
        Component.onCompleted: {
            chart.rePaint();
        }
    }

    MultiPointTouchArea {
        id: touchArea
        anchors.fill: parent
        minimumTouchPoints: 2
        maximumTouchPoints: 2
        mouseEnabled:true
        visible: root.active

        property real p1X: 0
        property real p2X: 0
        property real p1Y: 0
        property real p2Y: 0
        property real plen: 0

        function checkPointer(){

            var plen_ = Math.sqrt( Math.pow( touch1.x - touch2.x , 2 ) + Math.pow( touch1.y - touch2.y , 2))
            if( plen_ - touchArea.plen > 50 ){
                chart.changeSize( (touch1.x + touch2.x)/2 , 1 )
                touchArea.plen = Math.sqrt( Math.pow( touchArea.p1X - touchArea.p2X , 2 ) + Math.pow( touchArea.p1Y - touchArea.p2Y , 2) )
            }else if( plen_ - touchArea.plen < -50 ) {
                chart.changeSize( (touch1.x + touch2.x)/2 , -1 )
                touchArea.plen = Math.sqrt( Math.pow( touchArea.p1X - touchArea.p2X , 2 ) + Math.pow( touchArea.p1Y - touchArea.p2Y , 2) )
            }
        }
        touchPoints: [
            TouchPoint {
                id: touch1
            },
            TouchPoint {
                id: touch2
                onPressedChanged: {

                    if( pressed ){
                        touchArea.p1X = touch1.startX
                        touchArea.p2X = touch2.startX
                        touchArea.p1Y = touch1.startY
                        touchArea.p2Y = touch2.startY
                        touchArea.plen = Math.sqrt( Math.pow( touchArea.p1X - touchArea.p2X , 2 ) + Math.pow( touchArea.p1Y - touchArea.p2Y , 2) )
                    }
                }
                onXChanged: {
                    touchArea.checkPointer();
                }
                onYChanged: {
                    touchArea.checkPointer();
                }
            }
        ]

        MouseArea{
            id: mouse_
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            acceptedButtons: Qt.AllButtons
            cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
            property bool isMove: false
            property real x_: 0

            onPositionChanged: {//获取鼠标移动时的相对位置
                if( mouse.x <= width && mouse.x >= 0 ){
                    chart.linePos = mouse.x
                    // lineChart.reflash()
                }
            }
            onExited:{ //检测鼠标是否离开图表区域
                chart.linePos = -1;
            }
            onWheel: {
                if( spchart.isChangeSize ) return;
                spchart.isChangeSize = true
                if( wheel.angleDelta.y > 0 ){
                   // 放大
                   chart.changeSize( wheel.x , 1 );
                }else{
                   // 缩小
                   chart.changeSize( wheel.x , -1 );
                }
                spchart.isChangeSize = false
            }

            onMouseXChanged: {
                if( !mouse_.pressed ){
                    return false;
                }

                var x_ = mouseX

                if( x_ < 0 || x_ > width ){
                    return;
                }

                if( Math.abs(x_ - mouse_.x_) > 10 && mouse_.x_ > 0 ){
                    mouse_.isMove = true;
                    chart.move( mouse_.x_ , x_ );
                }
                mouse_.x_ = mouseX
            }

            onClicked:{
                if(mouse.button == Qt.MidButton || mouse.button == Qt.MiddleButton ){
                   // 重置
                    chart.reSize();
                }
                if (mouse.button === Qt.RightButton) {
                    option_menu.popup()
                }
            }

            onPressAndHold:{
                if( !mouse_.isMove ){
                    option_menu.popup()
                }
            }

            onPressed: {
                chart.linePos = mouse.x
                mouse_.x_ = 0
            }

            onReleased: {
                mouse_.isMove = false;
                mouse_.x_ = 0
                chart.linePos = -1;
            }

            Menu {
                id: option_menu
                width: 150 / root.dpi

                MenuItem {
                    id: menu_export
                    height: 40 / root.dpi
                    text: "导出数据到csv"
                    visible: root.sys != "linux"
                    enabled: dataContent.file != "" || dataContent.isStop? true : false
                    onTriggered: {
                        folderDialog.open();
                    }
                }

                MenuItem {
                    id: menu_save_csv
                    height: 40 / root.dpi
                    text: "保存数据到csv"
                    visible: true
                    enabled: dataContent.file != "" || dataContent.isStop? true : false
                    onTriggered: {
                        if (dm.saveEvToCSV("eigenvalue",spchart.channelId,root.channelName[spchart.channelId],"特征值" , spchart.evType, dataContent.file!="")){
                            messageDialog.show("保存成功");
                        }else{
                            messageDialog.show("保存失败");
                        }
                    }
                }

                MenuItem {
                    text: "坐标复位"
                    height: 40 / root.dpi
                    onTriggered: {
                        chart.reSize()
                    }
                }

                Component.onCompleted: {
                    if( root.sys == "linux" ){
                        option_menu.removeItem( menu_export )
                    }else{
                        // option_menu.removeItem( menu_save_csv )
                    }
                }

            }
        }
    }

    FolderDialog {
         id: folderDialog
         currentFolder: ""
         acceptLabel: "确定"
         rejectLabel: "取消"
         options: FolderDialog.ShowDirsOnly
         folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
         onAccepted: {
            dm.exportEvToCSV(folderDialog.currentFolder , spchart.channelId, root.channelName[spchart.channelId] , "特征值" , spchart.evType, dataContent.file!="")
         }
    }

    function start(){

        spchart.dataArr = [];
        spchart.maxArr = [];
        spchart.initChart()
        spchart.isStop = false;

    }

    function stop(){
        spchart.isStop = true;
    }

    function initChart(){
        if( spchart.disabled ) return;

        spchart.times = 0
        spchart.seconds = 10
        chart.clearData();
        chart.resetCurveLine()
    }

    // 打开历史文件
    function setData( data ){

        if( spchart.disabled ) return;
        var max = axisY.max
        var max_ = 0

        chart.clearData();

        var isFull = false;

    }

    // 添加相对时间轴数据
    function addXYData( data ){

        if( spchart.disabled ) return;
        var max = spchart.ymax
        var max_ = 0

        let max__ = -Infinity;
        let isOut = false;
        let isFull = false;

        for (var i = 0; i < data.length; ++i)
        {
            spchart.dataArr.push( data[i] );
            if( data[i] > max__ ) max__ = data[i];
            if( spchart.dataArr.length > spchart.unitLength ){
                spchart.dataArr.shift()
                isOut = true;
            }
        }
        spchart.maxArr.push( max__ );

        if( isOut && spchart.maxArr.length > 1 ){
            spchart.maxArr.shift();
        }

        if( spchart.dataArr.length > 0 ){
            max_ = Math.max(...spchart.maxArr);

            if ( spchart.ymax != max_ ){
                chart.yAxisSelfChange( max_ , 0 , 2 );
                spchart.ymax = chart.yAxisMax
            }
            spchart.times++;
        }
    }

    Component.onCompleted:{
        spchart.initChart();
    }

}
