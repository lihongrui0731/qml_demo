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
    property string channelId: "sound"
    property string sourseType: "sound"

    property real frameCount: 2000
    property real second: 10
    property real dt: 0.005
    property real unitLength: chart.second / chart.dt
    property real xmin: -chart.second
    property real xmax: 0
    property real unit: (chart.xmax - chart.xmin) / chart.unitLength
    property real ymin: 0
    property real ymax: 1
    property real currentIndex: 0
    property real linePos: 0;
    property var dataArr: []
    property int times: 0;
    property bool isChangeSize: false
    property var maxArr:[];
    property int fontSize: 10/root.dpi
    property bool touchFun: true
    property string backgroundColor: root.colorConfig["cardColor"]

    property bool isStop: false
    property string file: ""

    function checkStatus(){
        return chart.file != "" || chart.isStop? true : false
    }

    function setLeqMData( channelId_ , dt , leq ){
        if( channelId_ === chart.channelId && !chart.isChangeChannel  ){

            if( ( chart.dt !== dt && dt > 0 ) || chart.times == 0 ){
                if( dt > 0 ){
                    chart.dt = dt
                }
                lineChart.df = chart.unit;
                lineChart.rePaint();
            }
            chart.addData( [leq[0]] );
            var data__ = { max:chart.ymax , values: chart.dataArr };
            lineChart.addJsonBatchData( data__ );
        }
    }

    function setLeqData( channelId_ , d ){
        if( channelId_ === chart.channelId && !chart.isChangeChannel  ){
            // console.log( d );
            var json_ = JSON.parse( d );
            if( ( chart.dt !== json_["dt"] && json_["dt"] > 0 ) || chart.times == 0 ){
                if( json_["dt"] ){
                    chart.dt = json_["dt"];
                }
                lineChart.df = chart.unit;
                lineChart.rePaint();
            }
            chart.addData( json_["values"] );
            var data__ = { max:chart.ymax , values: chart.dataArr };
            lineChart.addJsonBatchData( data__ );
        }
    }

    Connections {
        target: root
        function onActiveChanged(){
            if( root.active ){
                lineChart.reInitial();
                //lineChart.reSize();

                var data__ = { max:chart.ymax , values: chart.dataArr };
                lineChart.addJsonBatchData( data__ );
            }
        }
    }

    Connections {
        target: dm

        function onLeqChanged( channelId_ , d ){
            chart.setLeqData( channelId_ , d )
        }

        function onSendFinalLeq( channelId_ , json_ ){
            chart.maxArr = [];
            chart.dataArr = [];
            lineChart.clearData();

            if( channelId_ === chart.channelId ){
                for( let k in json_ ){
                    chart.addData( json_[k]["values"] );
                }

                chart.second = Math.round(chart.dataArr.length * chart.dt);
                lineChart.reInitial();

                var data__ = { max:chart.ymax , values: chart.dataArr };
                lineChart.addJsonBatchData( data__ );
            }
        }

    }

    function setData( json_ ){
        if( ( chart.dt !== json_[0]["dt"] && json_[0]["dt"] > 0 ) || chart.times == 0 ){
            chart.dt = json_[0]["dt"];

            lineChart.df = chart.unit;
            lineChart.rePaint();
        }

        for( let k in json_ ){
            chart.addData( json_[k]["values"] );
        }
        chart.second = Math.round(chart.dataArr.length * chart.dt);
        lineChart.reInitial();

        var data__ = { max:chart.ymax , values: chart.dataArr };
        lineChart.addJsonBatchData( data__ );
    }

    function addData( data ){
        var max = chart.ymax
        var max_ = 0
        var isFull = false;

        let max__ = 0;
        let isOut = false;
        for( var i = 0; i < data.length; i++ ){
            chart.dataArr.push( data[i] );
            if( data[i] > max__ ) max__ = data[i];
            if( chart.dataArr.length > chart.unitLength ){
                chart.dataArr.shift();
                isOut = true;
            }
        }
        chart.maxArr.push(max__);
        if( isOut ){
            chart.maxArr.shift();
        }
        max_ = Math.max(...chart.maxArr);

        if ( chart.ymax != max_ ){
            lineChart.yAxisSelfChange( max_ , 0 , 2 );
            chart.ymax = lineChart.yAxisMax
        }
        chart.times++;
    }

    function getDataFromOpenData(){
        // zzzz
    }

    function setChannel( channelId_ ){
        if( 1 || channelId_ !== chart.channelId ){
            chart.isChangeChannel = true;
            chart.channelId = channelId_

            lineChart.clearData();
            lineChart.reInitial();
            /*
            if( data_vibration.file != "" ){
                dm.getTimedataFromFile( chart.channelId )
            }else{

                if( data_vibration.isStop ){
                    dm.getTimedata( chart.channelId )
                }
            }
            */
            chart.isChangeChannel = false
        }
    }

    function start(){
        chart.times = 0
        chart.second = 10

        chart.dataArr = [];
        chart.maxArr = [];
        lineChart.clearData();
        lineChart.resetCurveLine()
    }

    function stop(){
    }

    function setFrameCount( channelId_ , count ){
        if( channelId_ === chart.channelId ){
            chart.frameCount = count
        }
    }

    function changeTimeLine( timeline , currentFrame ){

        var currentIndex_ = currentFrame / chart.frameCount * chart.unitLength;
        if( currentIndex_ != chart.currentIndex ){
            chart.currentIndex = currentIndex_;
        }

        lineChart.setCurrentPos( currentIndex_ );
    }

    CLineChart{
        id: lineChart
        anchors.fill: parent
        xAxisMax: chart.xmax
        xAxisMin: chart.xmin
        yAxisMax: chart.ymax
        yAxisMin: 0
        xAxisTitle: qsTr("时间（s）");
        yAxisTitle: qsTr("dB");
        dpi: root.dpi
        fontSize: chart.fontSize
        frameLen: chart.unitLength
        bgColor: chart.backgroundColor
        textColor: "#aaaaaa"
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
            lineChart.exportCSV(folderDialog.currentFolder, root.channelName[chart.channelId] , "leq")
         }
    }

    MultiPointTouchArea {
        id: touchArea
        anchors.fill: parent
        minimumTouchPoints: 2
        maximumTouchPoints: 2
        mouseEnabled:true
        visible: root.active && chart.touchFun

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
                    visible: root.sys !== "linux"
                    enabled: chart.checkStatus();
                    onTriggered: {
                        folderDialog.open();
                    }
                }

                MenuItem {
                    id: menu_save_csv
                    height: 40 / root.dpi
                    text: "保存数据到csv"
                    visible: true
                    enabled: chart.checkStatus();
                    onTriggered: {
                        if (lineChart.saveCSV("timedata",root.channelName[chart.channelId],"leq")){
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
                    if( root.sys === "linux" ){
                        option_menu.removeItem( menu_export )
                    }else{
                        option_menu.removeItem( menu_save_csv )
                    }
                }

            }
        }
    }


}
