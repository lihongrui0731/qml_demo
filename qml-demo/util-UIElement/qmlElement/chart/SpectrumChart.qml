import QtQuick 2.15
import Qt.labs.platform 1.0
import QtQuick.Controls 2.15
import CLineChart 1.0

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config
import '../../resources/js/public.js' as JS

Item {
    id:chart

    property real currentTimeLine: 0
    property string channelId: "sound"
    property string sourseType: "sound"

    property bool isChangeChannel: false
    property bool isChangeSize: false

    property real dt: 0.2
    property real unitLength: 800
    property real xmin: 0
    property real xmax: chart.unitLength / chart.dt
    property real unit: chart.xmax / chart.unitLength
    property real ymin: 0
    property real ymax: 5
    property real currentIndex: 0
    property real linePos: 0;
    property var dataArr: new Array(chart.unitLength).fill(0);
    property int times: 0;
    property int times_: 0; // 记录总次数 第一次的时候 刷新参数

    property int fontSize: 10/root.dpi
    property string backgroundColor: root.colorConfig["cardColor"]

    function addSpectraJson( channelId_ , json_ ){

//        if( root.isHide || !root.isVibration ){
//            return
//        }
        if( channelId_ === chart.channelId && !chart.isChangeChannel ){
            // var json_ = JSON.parse( d );
            if( ( chart.dt !== json_["dt"] && json_["dt"] > 0 ) || chart.times_ == 0 ){

                lineChart.frameLen = chart.unitLength = json_["bandWidth"] / json_["deltaFreq"];
                lineChart.dt = chart.dt = json_["dt"];
                lineChart.xAxisMax = chart.xmax = json_["bandWidth"]
                lineChart.df = chart.unit = json_["deltaFreq"];
                lineChart.rePaint();
                chart.dataArr = new Array(chart.unitLength).fill(0);
                chart.times = 0;
            }

            chart.addData( json_["values"] , json_["frameCount"] , json_["frameLength"] );

            var data__ = { max:chart.ymax , values: chart.dataArr };

            lineChart.addJsonData( data__ );
            // lineChart.addData( json_["values"] , json_["frameCount"] );
            chart.times_++;
        }
    }

    Connections {
        target: root
        function onActiveChanged(){
            if( root.active ){
                // lineChart.reSize()
                chart.isChangeChannel = true;
                lineChart.reInitial();
                lineChart.reSize();
                chart.isChangeChannel = false;
            }
        }

        function onIsVibrationChanged(){
            if( root.isVibration ){
                chart.isChangeChannel = true;
                lineChart.reInitial();
                lineChart.reSize();
                chart.isChangeChannel = false;
            }
        }
    }

    Connections {
        target: dm

        function onSpectraJsonChanged( channelId_ , json_ ){

            if( root.isHide || !root.isVibration ){
                return
            }
            if( channelId_ === chart.channelId && !chart.isChangeChannel ){
                // var json_ = JSON.parse( d );
                if( ( chart.dt !== json_["dt"] && json_["dt"] > 0 ) || chart.times_ == 0 ){

                    lineChart.frameLen = chart.unitLength = json_["bandWidth"] / json_["deltaFreq"];
                    lineChart.dt = chart.dt = json_["dt"];
                    lineChart.xAxisMax = chart.xmax = json_["bandWidth"]
                    lineChart.df = chart.unit = json_["deltaFreq"];
                    lineChart.rePaint();
                    chart.dataArr = new Array(chart.unitLength).fill(0);
                    chart.times = 0;
                }

                chart.addData( json_["values"] , json_["frameCount"] , json_["frameLength"] );

                var data__ = { max:chart.ymax , values: chart.dataArr };

                lineChart.addJsonData( data__ );
                // lineChart.addData( json_["values"] , json_["frameCount"] );
                chart.times_++;
            }
        }

        function onSendSpectraFromFile( channelId_ , json_ , len ){
            // 获取振动历史文件多通道数据
            if( channelId_ === chart.channelId ){
                // var json_ = JSON.parse( data_ );

                if( ( chart.dt !== json_["dt"] && json_["dt"] > 0 ) || chart.times_ == 0 ){

                    chart.unitLength = json_["bandWidth"] / json_["deltaFreq"];
                    chart.dt = json_["dt"];
                    chart.xmax = json_["bandWidth"]
                    lineChart.df = chart.unit = json_["deltaFreq"];
                    lineChart.rePaint();
                }

                chart.times = 0
                chart.dataArr = new Array(chart.unitLength).fill(0);

                chart.addData( json_["values"] , json_["frameCount"] , json_["frameLength"] );
                var data__ = { max:chart.ymax , values: chart.dataArr };

                lineChart.addJsonData( data__ );
                // lineChart.addData( json_["values"] , json_["frameCount"] );
                chart.times_++;
                lineChart.reSize();
            }
        }

        // 结束后 获取历史数据
        function onSendSpectra( data_ , channelId_ ){
            if( data_ == "{}"){
                return
            }

            if( channelId_ === chart.channelId && !chart.isChangeChannel ){
                chart.isChangeChannel = true
                // var json_  = JSON.parse( data_[0] );
                var json_  = JSON.parse( data_[0] );

                if( chart.times == 0 && chart.dt !== json_["dt"] && json_["dt"] > 0 ){
                    chart.unitLength = json_["bandWidth"] / json_["deltaFreq"];
                    chart.dt = json_["dt"];
                    chart.xmax = json_["bandWidth"]

                    lineChart.df = chart.unit = json_["deltaFreq"];
                    lineChart.rePaint();
                }
                chart.dataArr = new Array(chart.unitLength).fill(0);
                chart.times = 0;

                for( let i = 0; i < data_.length; i ++ ){
                    json_ = JSON.parse( data_[i] );
                    chart.addData( json_["values"] , json_["frameCount"] , json_["frameLength"] );
                }

                var data__ = { max:chart.ymax , values: chart.dataArr };
                lineChart.addJsonData( data__ );
                chart.isChangeChannel = false
            }
        }

    }

    // 稳态fft 每次数据累加做平均
    function addData( data , count , frameLen ){
        var max = chart.ymax
        var max_ = 0
        var min_ = Infinity;
        var y = max;
        var y_ = 0;

        var isFull = false;

        var n = chart.times > 0 ? 9 : 0

        for( var k = 0; k < count; k++ ){
            // data[k][0] = data[k][1];
            for (var i = 0; i < frameLen && i < chart.unitLength ; ++i)
            {
                // chart.dataArr[i] = data[k][i];
                // chart.dataArr[i] = (chart.dataArr[i] * chart.times + data[k][i] * 10 ) / ( chart.times + 10 );
                chart.dataArr[i] = (chart.dataArr[i] * n + data[k][i] ) / ( n + 1 );

                if( chart.dataArr[i] > max_ ){
                    max_ = chart.dataArr[i];
                }
                if(  chart.dataArr[i] < min_ ){
                    min_ = chart.dataArr[i];
                }
            }
        }

        if( lineChart.isDB ){
            max_ = 20 * Math.log10( max_ )
            min_ = 20 * Math.log10( min_ )

            if ( chart.ymax != max_ ){
                lineChart.yAxisSelfChange( max_ , min_ , 5 );
                chart.ymax = lineChart.yAxisMax
            }
        }else{
            if ( chart.ymax != max_ ){
                lineChart.yAxisSelfChange( max_ , 0 , 5 );
                chart.ymax = lineChart.yAxisMax
            }
        }
        chart.times++;
    }

    function getDataFromOpenData(){
        chart.dataArr = new Array(chart.unitLength).fill(0);
        chart.times = 0;
        // dm.getSpectraFromFile( chart.channelId )
    }

    function setChannel( channelId_ ){
        if( 1 || channelId_ !== chart.channelId ){
            chart.isChangeChannel = true;
            chart.channelId = channelId_

            if( data_vibration.file != "" ){
                chart.dataArr = new Array(chart.unitLength).fill(0);
                chart.times = 0;
                lineChart.clearData();
                lineChart.reInitial();
                // dm.getSpectraFromFile( chart.channelId )
            }else{
                chart.dataArr = new Array(chart.unitLength).fill(0);
                chart.times = 0;
                lineChart.clearData();
                lineChart.reInitial();
                if( data_vibration.isStop  ){
                //    dm.getSpectra( chart.channelId )
                }
            }
            chart.isChangeChannel = false;
        }
    }

    function start(){
        chart.times = 0
        lineChart.resetCurveLine()
    }

    function stop(){
    }

    function changeTimeLine( timeline , currentFrame ){

        chart.currentIndex = currentFrame / chart.frameCount * chartView.ys.length;
        var value_;
        var pos;

        if( chart.linePos != pos.x ){
            chart.linePos = pos.x
        }
    }

    CLineChart{
        id: lineChart
        anchors.fill: parent
        xAxisMax: chart.xmax
        xAxisMin: chart.xmin
        yAxisMax: 1
        yAxisMin: chart.ymin

        xAxisTitle: qsTr("频率（Hz）");
        yAxisTitle: qsTr("幅值（dB(A)）");
        dpi: root.dpi
        fontSize: chart.fontSize
        textColor: "#aaaaaa"
        bgColor: chart.backgroundColor

        frameLen: chart.unitLength
        Component.onCompleted: {
            lineChart.rePaint();
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
            lineChart.exportCSV(folderDialog.currentFolder , root.channelName[chart.channelId] , "频域")
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
                lineChart.changeSize( (touch1.x + touch2.x)/2 , 1 )
                touchArea.plen = Math.sqrt( Math.pow( touchArea.p1X - touchArea.p2X , 2 ) + Math.pow( touchArea.p1Y - touchArea.p2Y , 2) )
            }else if( plen_ - touchArea.plen < -50 ) {
                lineChart.changeSize( (touch1.x + touch2.x)/2 , -1 )
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
            cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
            propagateComposedEvents: true
            acceptedButtons: Qt.AllButtons
            property real x_: 0
            property bool isMove: false

            onPositionChanged: {    //获取鼠标移动时的相对位置
                if( mouse.x <= width && mouse.x >= 0 ){
                    lineChart.linePos = mouse.x
                  //  lineChart.reflash()
                }
            }
            onExited:{ //检测鼠标是否离开图表区域
                lineChart.linePos = -1;
            }
            onWheel: {
                if( chart.isChangeSize ) return;
                chart.isChangeSize = true;
                if( wheel.angleDelta.y > 0 ){
                   // 放大
                   lineChart.changeSize( wheel.x , 1 );
                }else{
                   // 缩小
                   lineChart.changeSize( wheel.x , -1 );
                }
                chart.isChangeSize = false
            }

            onClicked: {
                if(mouse.button == Qt.MidButton || mouse.button == Qt.MiddleButton ){
                   // 重置
                    lineChart.reSize();
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
                lineChart.linePos = mouse.x
                mouse_.x_ = 0
            }
            onReleased: {
                mouse_.isMove = false;
                mouse_.x_ = 0
                lineChart.linePos = -1;
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
                    lineChart.move( mouse_.x_ , x_ );
                }
                mouse_.x_ = mouseX
            }

            Menu {
                id: option_menu
                width: 150 / root.dpi

                MenuItem {
                    id: dbAxis
                    height: 30 / root.dpi
                    text: "dB坐标"
                    checkable: true
                    onTriggered: {
                        var max_ = 0;
                        var min_ = 0;
                        if( !lineChart.isDB ){
                             lineChart.yAxisTitle = "dB";
                             lineChart.isDB = true;
                             lineChart.isLG = false;

                             if( Math.max(...dataArr) != 0 ){
                                 max_ = 20 * Math.log10( Math.max(...dataArr) )
                                 min_ = 20 * Math.log10( Math.min(...dataArr) )
                             }
                             if( max_ !== 0 ){
                                lineChart.yAxisSelfChange( max_ , min_ , 5 );
                             }

                        }else{
                            lineChart.yAxisTitle = "加速度（g）";
                            lineChart.isDB = false;
                            lineChart.isLG = false;

                            max_ = Math.max(...dataArr);
                            if( max_ !== 0){
                                lineChart.yAxisSelfChange( max_, 0 , 5 );
                            }

                        }
                        lgAxis.checked = false
                        lineChart.rePaint()
                    }
                }

                MenuItem {
                    id: lgAxis
                    height: 40 / root.dpi
                    text: "lg坐标"
                    checkable: true
                    onTriggered: {
                        var max_;
                        if( !lineChart.isLG ){
                            lineChart.isLG = true;
                        }else{
                            lineChart.isLG = false;
                        }

                        lineChart.isDB = false;
                        lineChart.yAxisTitle = "加速度（g）";
                        max_ = Math.max(...dataArr);

                        if( max_ !== 0){
                            lineChart.yAxisSelfChange( max_, 0 , 5 );
                        }
                        dbAxis.checked = false
                        lineChart.rePaint()

                    }
                }

                MenuItem {
                    id: menu_export
                    height: 40 / root.dpi
                    text: "导出数据到csv"
                    visible: root.sys != "linux"
                    enabled: data_vibration.file != "" || data_vibration.isStop? true : false
                    onTriggered: {
                        folderDialog.open();
                    }
                }

                MenuItem {
                    id: menu_save_csv
                    height: 40 / root.dpi
                    text: "保存数据到csv"
                    visible: true
                    enabled: data_vibration.file != "" || data_vibration.isStop? true : false
                    onTriggered: {
                        if (lineChart.saveCSV("fft",root.channelName[chart.channelId],"频域")){
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
                        lineChart.reSize()
                    }
                }

                Component.onCompleted: {
                    if( root.sys == "linux" ){
                        option_menu.removeItem( menu_export )
                    }else{
                        option_menu.removeItem( menu_save_csv )
                    }
                }
            }

        }

    }
}
