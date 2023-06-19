import QtQuick 2.15
import Qt.labs.platform 1.0
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config
import '../../resources/js/public.js' as JS

Item {
    id: lineAxis
    clip: true;
    property string title: "";
    property real lablefont: 10/root.dpi
    property bool vertical: false
    property real linePos: -1
    property real currentIndex: 0

    // 反边距
    property real borderMargins: 5 / root.dpi

    signal timeLineChanged( int currentIndex );

    function setData( data_ ){
        chart.dataArr = data_
        line.clear();
        axisX.max = chart.dataArr.length

        axisY.min = Math.floor( Math.min(...chart.dataArr))
        axisY.max = Math.ceil( Math.max(...chart.dataArr))

        for (var i = 0; i < chart.dataArr.length; ++i)
        {
            line.append( i+1, chart.dataArr[i] )
        }
    }

    function changeTimeLine( currentFrame ){
        lineAxis.currentIndex = currentFrame;

        var value_ = line.at( currentFrame );
        var pos = chart.mapToPosition( value_ , line );

        if( lineAxis.linePos != pos.x ){
            lineAxis.linePos = pos.x
            lineAxis.drawTimeLine()
        }
    }

    ChartView {
        id: chart
        x: lineAxis.vertical? -lineAxis.borderMargins + ( parent.width - parent.height) / 2:0
        y: lineAxis.vertical? -lineAxis.borderMargins + ( parent.height - parent.width) / 2:0
        width: lineAxis.vertical? parent.height + lineAxis.borderMargins*2 : parent.width + lineAxis.borderMargins*2
        height: lineAxis.vertical? parent.width + lineAxis.borderMargins*2 : parent.height + lineAxis.borderMargins*2

        anchors.margins: 0 / root.dpi
        antialiasing: false
        theme:ChartView.ChartThemeBlueNcs
        legend.visible : false
        margins.left: 0
        margins.right: 0
        margins.top: 0
        margins.bottom: 0

        transformOrigin: Popup.Center
        rotation: lineAxis.vertical?-90:0

        property int unit: 1 // 单位时间的点数
        property var dataArr:[]

        ValueAxis{
            id: axisX
            min: 0
            max: 1
            tickCount : 0
            labelsVisible: false
            gridVisible: false

        }

        ValueAxis{
            id: axisY
            min: 0
            max: 1
            tickCount : 2
            labelsVisible: true
            labelFormat: "%d"
            gridVisible: false
        }

        LineSeries
        {
            id : line
            axisX: axisX
            axisY: axisY
            color: UI_color[UI_config[root.usedConfig].colorType].normal_chart
            width: 1.0/root.dpi
        }

        Canvas{
            id: posCanvas
            anchors.fill: chart
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
                    ctx.moveTo(xx,chart.plotArea.y)
                    ctx.lineTo(xx,chart.plotArea.height + chart.plotArea.y)

                    ctx.font = fontMetrics.getFontToContext2D();
                    ctx.text( posCanvas.value , xx+50> posCanvas.width? xx -50: xx + 5 , (chart.plotArea.height + chart.plotArea.y) / 2 )

                    ctx.stroke()
                    ctx.fillStyle = UI_color[UI_config[root.usedConfig].colorType].normal_chart_line
                    ctx.fill()
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


            function setLinePos( x ){
                lineAxis.linePos = x;

                var point_ = chart.mapToValue(Qt.point(lineAxis.linePos,1),line)//使用mapToValue获得该鼠标位置处AreaSeries横坐标的值

                if( chart.dataArr.length > 0 ){
                    if( point_.x < 0 || point_.x > chart.dataArr.length ){
                         posCanvas.reset()
                         return;
                    }
                }

                var index = Math.round(point_.x);
                if( index === -1 ){
                    return;
                }
                lineAxis.currentIndex = index
                lineAxis.drawTimeLine();
            }

        }
    }

    MouseArea{
        id: mouse_
        anchors.fill: chart
        propagateComposedEvents: true
        cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
        property bool isMove: false
        property real x_: 0

        transformOrigin: Popup.Center
        rotation: lineAxis.vertical?-90:0

        onPositionChanged: {//获取鼠标移动时的相对位置
            posCanvas.setLinePos( mouse.x )
            lineAxis.timeLineChanged( lineAxis.currentIndex  )
        }

        onClicked: {
            posCanvas.setLinePos( mouse.x )
            lineAxis.timeLineChanged( lineAxis.currentIndex  )
        }
    }

    FontMetrics {
        id: fontMetrics
        font.pixelSize: 20;

        function getFontToContext2D() {
            var cssFontString = "";
            if(fontMetrics.font.italic) {
                cssFontString += "italic ";
            } else {
                cssFontString += "normal ";
            }

            if(fontMetrics.font.bold) {
                cssFontString += "bold ";
            } else {
                cssFontString += "normal ";
            }

            cssFontString += (fontMetrics.font.pixelSize+"px ");
            // cssFontString += ("/"+fontMetrics.height+"px ");
            cssFontString += fontMetrics.font.family;
            return cssFontString;
        }
    }

    function drawTimeLine(){
        if( lineAxis.disabled ) return;
        var index = lineAxis.currentIndex

        if( index < 0 || chart.dataArr.length == 0 ){
            return;
        }

        posCanvas.index = index;
        posCanvas.value = JS.scienceNumber( chart.dataArr[index] )
        posCanvas.xx = lineAxis.linePos
        posCanvas.requestPaint()
    }
}
