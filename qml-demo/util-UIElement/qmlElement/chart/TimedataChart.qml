import QtQuick 2.15
import Qt.labs.platform 1.1
import QtQuick.Controls 2.15
import CLineChart 1.0

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config
import '../../resources/js/public.js' as JS

Item {
    id: chart

    property real currentTimeLine: 0
    property string channelId: ""
    property string sourseType: "vibration"

    property bool isChangeChannel: false

    property real second: 1
    property real dt: 0
    property real unitLength: 1 / chart.dt
    property real xmin: 0
    property real xmax: chart.second * 1000
    property real unit: chart.xmax / chart.unitLength
    property real ymin: -1
    property real ymax: 1
    property real currentIndex: 0
    property real linePos: 0;
    property var dataArr: []
    property int times: 0;
    property bool isChangeSize: false

    property int fontSize: 10/root.dpi

    Connections {
        target: root
        function onActiveChanged(){
            if( root.active ){
                chart.isChangeChannel = true;
                lineChart.reInitial();
                lineChart.reSize();
                chart.isChangeChannel = false;
            }
        }
        function onIsVibrationChanged(){
            return;
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

        function onTimedataChanged( channelId_ , json_ ){
            if( root.isHide || !root.isVibration ){
                return
            }
            if( channelId_ === chart.channelId && !chart.isChangeChannel ){
                // var json_ = JSON.parse( d );
                if( ( chart.dt !== json_["dt"] && json_["dt"] > 0 ) || chart.times == 0 ){
                    chart.dt = json_["dt"];
                    chart.second = json_["values"].length * chart.dt;

                    chart.unitLength = json_["values"].length

                    lineChart.df = chart.unit;
                    lineChart.rePaint();
                }
                chart.addData( json_["values"] );
                var data__ = { max:chart.ymax , values: chart.dataArr };
                lineChart.addJsonData( data__ );
            }

        }

        function onSendTimedataFromFile( channelId_ , dt_ , list , len_ ,max_ ){
            if( channelId_ === chart.channelId ){

                chart.dt = dt_;
                chart.second = Math.round(len_ * chart.dt);
                chart.unitLength = len_
                lineChart.df = chart.unit;
                lineChart.rePaint();

                if( lineChart.isDB ){
                    max_ = 20 * Math.log10( max_ )
                }

                if( chart.ymax != max_ || chart.ymin != -max_ ){
                    lineChart.yAxisSelfChange( max_ , -max_ , 2 );
                    chart.ymax = lineChart.yAxisMax
                    chart.ymin = lineChart.yAxisMin
                }

                lineChart.addData( list , len_);
                lineChart.reSize();
            }
        }

        function onSendTimedataFromThread( data , dt , channelId_ , len ,  max ){
            if( channelId_ === chart.channelId ){
                chart.dt = dt;
                chart.second = Math.round(len * chart.dt);
                chart.unitLength = len
                lineChart.df = chart.unit;
                lineChart.rePaint();

                if( lineChart.isDB ){
                    max = 20 * Math.log10( max )
                }

                if( chart.ymax != max || chart.ymin != -max ){
                    lineChart.yAxisSelfChange( max , -max , 2 );
                    chart.ymax = lineChart.yAxisMax
                    chart.ymin = lineChart.yAxisMin
                }

                lineChart.addData( data , len);

            }
        }

    }

        /*
        function onSendTimedataFromFile( channelId_ , d ){

            if( d === "{}" ){
                return
            }

            if( channelId_ == chart.channelId && !isChangeChannel ){

                let float32 = new Float32Array([20.1,10.2]);
                 console.log( float32.byteLength );

                var json_ = JSON.parse( d );

                chart.dt = json_["dt"];
                chart.second = Math.round(json_["values"].length * chart.dt);
                chart.unitLength = json_["values"].length

                lineChart.df = chart.unit;

                lineChart.rePaint();

                chart.addData( json_["values"] );
                var data__ = { max:chart.ymax , values: chart.dataArr };

                lineChart.addJsonData( JSON.stringify( data__));
            }

        }*/


    function addData( data ){
        var max = chart.ymax
        var max_ = 0
        var isFull = false;

        chart.dataArr = data;
        var len = chart.dataArr.length;

        for( var i = 0; i < len; i++ ){
            if( Math.abs(data[i]) > max_ ){
                max_ = Math.abs(data[i]);
            }
        }

        if( lineChart.isDB ){
            max_ = 20 * Math.log10( max_ )
        }

        if ( chart.ymax != max_ || chart.ymin != -max_ ){
            lineChart.yAxisSelfChange( max_ , -max_ , 2 );
            chart.ymax = lineChart.yAxisMax
            chart.ymin = lineChart.yAxisMin
        }

        chart.times++;
    }

    function getDataFromOpenData(){
        dm.getTimedataFromFile( chart.channelId )
    }

    function setChannel( channelId_ ){
        if( 1 || channelId_ !== chart.channelId ){
            chart.isChangeChannel = true;
            chart.channelId = channelId_

            lineChart.clearData();
            lineChart.reInitial();

            if( data_vibration.file != "" ){

                dm.getTimedataFromFile( chart.channelId )
            }else{

                if( data_vibration.isStop ){
                    dm.getTimedata( chart.channelId )
                }
            }
            chart.isChangeChannel = false
        }
    }

    function start(){
        chart.times = 0
        chart.second = 1

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
        yAxisMax: chart.ymax
        yAxisMin: chart.ymin
        xAxisTitle: qsTr("时间（s）");
        yAxisTitle: qsTr("幅值（Pa）");
        dpi: root.dpi
        fontSize: chart.fontSize
        bgColor: root.cardColor
        textColor: "#aaaaaa"

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
            lineChart.exportCSV(folderDialog.currentFolder, root.channelName[chart.channelId] , "时域")
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
            propagateComposedEvents: true
            cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
            acceptedButtons: Qt.AllButtons
            property real x_: 0
            property bool isMove: false

            onPositionChanged: {    //获取鼠标移动时的相对位置
                if( mouse.x <= width && mouse.x >= 0 ){
                    lineChart.linePos = mouse.x
                    // lineChart.reflash()
                }
            }
            onExited:{ //检测鼠标是否离开图表区域
                lineChart.linePos = -1;
            }
            onWheel: {
                if( chart.isChangeSize ) return;
                chart.isChangeSize = true
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

            onPressed: {
                lineChart.linePos = mouse.x
                mouse_.x_ = 0
            }

            onReleased: {
                mouse_.isMove = false;
                mouse_.x_ = 0
                lineChart.linePos = -1;
            }

            onPressAndHold:{
                if( !mouse_.isMove ){
                    option_menu.popup()
                }
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
                        if (lineChart.saveCSV("timedata",root.channelName[chart.channelId],"时域")){
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
