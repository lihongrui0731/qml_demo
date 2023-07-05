import QtQuick 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config
import '../../resources/js/public.js' as JS

Item {
    id:chart
    anchors.fill: parent

    property real currentTimeLine: 0
    property string channelId: "ch1"
    property string sourseType: "vibration"

    property bool isChangeChannel: false

    property real dt: 0.2
    property int unitLength: 800
    property real xmin: 0
    property real xmax: chart.unitLength / chart.dt
    property real unit: chart.xmax / chart.unitLength
    property real ymin: 0
    property real ymax: 5
    property real currentIndex: 0
    property real linePos: 0;

    Connections {
        target: dm

        function onSpectraJsonChanged( channelId_ , json_ ){

            if( channelId_ == chart.channelId && !chart.isChangeChannel ){
                // var json_ = JSON.parse( d );
                chart.setSpectraData(channelId_ ,json_ );

            }
        }

        function onSendSpectraFromFile( channelId_ , json_ , len ){
            // 获取振动历史文件多通道数据
            if( channelId_ == chart.channelId ){
                // var json_ = JSON.parse( data_ );

                chart.setSpectraData(channelId_ ,json_ );
            }
        }
    }

    function setSpectraData( channelId_ , json_ ){
        if( chart.dt !== json_["dt"] && json_["dt"] > 0 ){
            chart.unitLength = json_["bandWidth"] / json_["deltaFreq"];
            chart.dt = json_["dt"];
            chart.xmax = json_["bandWidth"]
            chart.unit = json_["deltaFreq"];

            chartView.dataArr = new Array(chart.unitLength).fill(0);
        }

        chart.addData( json_["values"] , json_["frameCount"] );
    }

    function getDataFromOpenData(){
        dm.getSpectraFromFile( chart.channelId )
    }

    function setChannel( channelId_ ){
        if( channelId_ != chart.channelId ){
            chart.channelId = channelId_
            chart.initChart();
            line.clear()

            if( chart.file != "" ){
                dm.getSpectraFromFile( chart.channelId )
            }else{
                // dm.getSpectraData( chart.channelId )
            }
        }
    }

    function start(){
        chartView.dataArr = [];
        chart.initChart()
        line.clear()
    }

    function stop(){
    }

    function initChart(){
        axisY.max = 1
        chartView.dataArr = new Array( chart.unitLength ).fill(0);
    }

    // 稳态fft 每次数据累加做平均
    function addData( data , count ){
        var max = axisY.max
        var max_ = 0
        var y = max;
        var y_ = 0;

        var isFull = false;

        for( var k = 0; k < count; k++ ){
            for (var i = 0; i < data[k].length && i < chart.unitLength ; ++i)
            {
                chartView.dataArr[i] = data[k][i];
                //chartView.dataArr[i] = (chartView.dataArr[i] * chartView.times + data[k][i] * 10 ) / ( chartView.times + 10 );
                if( chartView.dataArr[i] > max_ ){
                    max_ = chartView.dataArr[i];
                }
            }
        }

        if( chartView.dataArr.length > 0 ){
            y_ =  max_ ;
            if( y != y_ ){
               axisY.max = y_ + y_ * 0.1
               line.clear()
            }

            for( var n = 0; n < chartView.dataArr.length; n++ ){
                var x = chart.unit * n;
                line.insert( n , x , chartView.dataArr[n] )
            }

        }

        chartView.times++;
    }

    function changeTimeLine( timeline , currentFrame ){

        chart.currentIndex = currentFrame / chart.frameCount * chartView.ys.length;
        var value_;
        var pos;
        value_ = line.at(chart.currentIndex)
        pos = chartView.mapToPosition( value_ , line );

        if( chart.linePos != pos.x ){
            chart.linePos = pos.x
            chart.drawTimeLine()
        }
    }

    function drawTimeLine(){

        var index = chart.currentIndex

        if( index < 0 ){
            return;
        }

        posCanvas.index = index;
        posCanvas.value = JS.scienceNumber( chartView.dataArr[index] )

        //var value = line.at(posCanvas.index)
        // var chartPosition = chart.mapToPosition(value,line)//将该值通过mapToPosition获得该值对应点位置（X2，Y2）
        posCanvas.xx = chart.linePos

        //posCanvas.yy = chartPosition.y
        posCanvas.requestPaint()

    }


    ChartView {
        id: chartView
        anchors.fill: parent
        antialiasing: true
        theme:ChartView.ChartThemeBlueNcs
        legend.visible : false
        margins.left: 0
        margins.right: 0
        margins.top: 0
        margins.bottom: 0
        property real lablefont: 8/root.dpi
        property real titleFont: 10/root.dpi
        property var dataArr:[chart.unitLength];
        property real dt: chart.dt
        property int unitLength: chart.unitLength
        property int times: 0

        ValueAxis{
            id: axisX
            min: chart.xmin
            max: chart.xmax
            tickCount : 11
            labelsFont.pixelSize: chartView.lablefont
            // titleText: "赫兹（Hz）"
            // titleFont.pixelSize: chartView.titleFont
            // titleFont.bold: true
        }

        Label{
            text: "赫兹（Hz）"
            font.pixelSize: chartView.titleFont
            font.bold: true
            anchors.bottom: chartView.bottom
            anchors.horizontalCenter: chartView.horizontalCenter
            anchors.bottomMargin: 5
        }

        ValueAxis{
            id: axisY
            min: chart.ymin
            max: chart.ymax
            tickCount : 5
            labelsFont.pixelSize: chartView.lablefont
            // titleText: qstr("加速度（g）")
            // titleFont.pixelSize: chartView.titleFont
            // titleFont.bold: true
        }

        Label{
            text: "加速度（g）"
            font.pixelSize: chartView.titleFont
            font.bold: true
            anchors.left: chartView.left
            anchors.verticalCenter: chartView.verticalCenter
            anchors.leftMargin: 0
            rotation:-90
            width:20/root.dpi
        }

        LineSeries {
            id : line
            axisX: axisX
            axisY: axisY
            color: UI_color[UI_config[root.usedConfig].colorType].normal_chart
            useOpenGL: true
            visible: true
        }

        Canvas{
            id: posCanvas
            anchors.fill: chartView
            property double xx: 0
            property double yy: 0
            property double index: 0
            property string value: ""
            onPaint: {
                var ctx = getContext("2d")//绘制十字交叉的竖线
                if(xx > 0 ){ // yy 画横线 暂时不需要

                    ctx.clearRect(0,0,parent.width,parent.height)
                    ctx.strokeStyle = UI_color[UI_config[root.usedConfig].colorType].normal_chart_line
                    ctx.lineWidth = 1/root.dpi
                    ctx.beginPath()
                    ctx.moveTo(xx,chartView.plotArea.y)
                    ctx.lineTo(xx,chartView.plotArea.height + chartView.plotArea.y)

                    ctx.text( "x: " + posCanvas.index * chart.unit , xx+50> posCanvas.width? xx -50: xx + 5 , (chartView.plotArea.height + chartView.plotArea.y) / 2 - 20/root.dpi )
                    ctx.text( "y: " + posCanvas.value , xx+50> posCanvas.width? xx -50: xx + 5 , (chartView.plotArea.height + chartView.plotArea.y) / 2 )

                    ctx.stroke()
                    /*
                    ctx.beginPath()//绘制十字交叉的横线
                    ctx.moveTo(chart.plotArea.x,yy)
                    ctx.lineTo(chart.plotArea.x + chart.plotArea.width,yy)
                    ctx.stroke()
                    */
                }else{//鼠标离开图表区域时，清除十字线
                    ctx.clearRect(0,0,parent.width,parent.height)
                }
            }

            function reset(){
                posCanvas.xx = 0
                posCanvas.yy = 0
                posCanvas.index = 0
                posCanvas.requestPaint()
                posCanvas.value = ""
            }
        }
    }

    MouseArea{
        anchors.fill: chartView
        hoverEnabled: true
        propagateComposedEvents: true
        cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
        onPositionChanged: {//获取鼠标移动时的相对位置
            chart.linePos = mouse.x;
            var point_ = chartView.mapToValue(Qt.point(chart.linePos,1),line)//使用mapToValue获得该鼠标位置处AreaSeries横坐标的值
            if( chartView.dataArr.length > 0 ){
                if( point_.x < 0 || point_.x > chart.xmax ){
                     posCanvas.reset()
                     return;
                }
            }

            var index = Math.round( point_.x / chart.xmax * chart.unitLength )
            if( index === -1 ){
                return;
            }

            chart.currentIndex = index
            chart.drawTimeLine();

        }
        onExited:{ //检测鼠标是否离开图表区域
            posCanvas.reset();
        }
    }


    Component.onCompleted:{
        chart.initChart();
    }

}
