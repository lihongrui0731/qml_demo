import QtQuick 2.15
import Qt.labs.platform 1.0
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config
import '../../resources/js/public.js' as JS

Item {
    id: leq_chart
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

    property real ymin: 0
    property real ymax: 1
    property string sourseType: "sound"
    //property int seconds: leq_chart.sourseType=="sound"?dm.getSoundRecordTime()/1000:dm.getVibrationRecordTime()/1000
    property int seconds: 10
    property real xmin: -leq_chart.seconds
    property real xmax: 0

    property string lastTime:"" //上一次上报数据取值时间
    property bool reprocess: false

    property bool disabled: false
    property bool unique: false

    property bool isStop: false

    signal reprocessFinish;

    onReprocessChanged: {
        if( leq_chart.reprocess ){
            leq_chart.changeAxisType("x")
        }else{
            // leq_chart.changeAxisType("date")
            leq_chart.changeAxisType("x")
        }
    }

    function setFrameCount( channelId_ , count , time ){
        if( channelId_ === leq_chart.channelId ){
            leq_chart.frameCount = count
            leq_chart.seconds = Math.round(time / 1000);

        }
    }

    function changeAxisType( type ){
        if( leq_chart.disabled ) return;
        chart.dataArr = [];
        chart.ys = [];
        chart.ysMax = [];
        line.clear();
        rline.clear();
        if( type === "x" ){
            line.visible = false;
            rline.visible = true;
            axisDate.visible = false
            axisX.visible = true
        }else{
            line.visible = true;
            rline.visible = false;
            axisDate.visible = true
            axisX.visible = false
        }
    }

    function setChannel( channelId_ ){
        if( leq_chart.disabled ) return;

        if( 1 || leq_chart.channelId !== channelId_ ){
            leq_chart.channelId = channelId_
            if( dataContent.file != "" ){
                line.clear();
                rline.clear();
                chart.ys = [];
                chart.ysMax = [];
                chart.dataArr = [];

                dm.getEigenvaluesByType( leq_chart.channelId , leq_chart.type );
            }else{
                dm.getEigenvalueData( leq_chart.channelId , leq_chart.type )
            }
        }
    }

    // 实时切换特征值
    function setType( value , unity ){
        if( leq_chart.unique ) return;
        if( leq_chart.disabled ) return;
        if( leq_chart.type !== value ){
            leq_chart.type = value
            leq_chart.unity = unity
            line.clear();
            rline.clear();
            chart.ys = [];
            chart.ysMax = [];
            chart.dataArr = [];
            dm.getEigenvalueData( leq_chart.channelId , leq_chart.type )
        }
        // 可能要清空数据 可能要读取历史数据
    }

    function getDataFromOpenData(){
        if( leq_chart.disabled ) return;
        line.clear();
        rline.clear();
        chart.ys = [];
        chart.ysMax = [];
        chart.dataArr = [];

        leq_chart.reprocess = true;
        leq_chart.changeAxisType("x");
        dm.getEigenvaluesByType( leq_chart.channelId , leq_chart.type );
    }

    // 打开保存文件 切换特征值
    function changeType( type , unity ){

        if( leq_chart.disabled ) return;
        if( leq_chart.type !== type ){
            leq_chart.type = type
            leq_chart.unity = unity
            rline.clear();
            line.clear();
            chart.dataArr = [];
            chart.ys = [];
            chart.ysMax = [];
            dm.getEigenvaluesByType( leq_chart.channelId , leq_chart.type );
        }
    }

    function doAddData( data ){
        if( leq_chart.disabled ) return;
        var data_ = JSON.parse(data);
        if( data_.dt * 1000 != chart.times ){
            // 频率发生改变
            chart.times = parseInt(data_.dt * 10) * 100;
            if( axisX.visible ){
                chart.x_unit = data_.dt;
                chart.count_max = leq_chart.seconds / chart.x_unit + 1;
            }else{
                chart.x_unit = Math.floor( chart.times / chart.unit );
                chart.count_max = chart.seconds * Math.floor( 1000 / chart.x_unit ) + 1
            }
        }

        if( axisX.visible ){
            if( leq_chart.type == "Level_A" ){
                leq_chart.addXYData( data_["values"] );
            }else{
                leq_chart.addXYData( data_["values"][leq_chart.type] );
            }
        }else{
            chart.curddate = new Date(data_.baseTime);
            if( leq_chart.type == "Level_A" ){
                leq_chart.addData( data_["values"] );
            }else{
                leq_chart.addData( data_["values"][leq_chart.type] );
            }
        }
    }

    function changeTimeLine( timeline , currentFrame ){
        if( leq_chart.disabled ) return;
        leq_chart.currentIndex = currentFrame / leq_chart.frameCount * chart.ys.length;

        var value_;
        var pos;
        if( axisX.visible ){
            var current_ = chart.ys.length - leq_chart.currentIndex < 1? 0: chart.ys.length - leq_chart.currentIndex - 1
            value_ = rline.at( current_ );
            pos = chart.mapToPosition( value_ , rline );
        }else{
            value_ = line.at(leq_chart.currentIndex)
            pos = chart.mapToPosition( value_ , line );
        }
        if( leq_chart.linePos != pos.x ){
            leq_chart.linePos = pos.x
            leq_chart.drawTimeLine()
        }
    }

    function drawTimeLine(){

        if( leq_chart.disabled ) return;
        var index = leq_chart.currentIndex

        if( index < 0 || chart.dataArr.length == 0 ){
            return;
        }

        posCanvas.index = index;
        posCanvas.value = JS.scienceNumber( chart.dataArr[index].y )

        // var value = line.at(posCanvas.index)
        // var chartPosition = chart.mapToPosition(value,line)//将该值通过mapToPosition获得该值对应点位置（X2，Y2）
        posCanvas.xx = leq_chart.linePos

        // posCanvas.yy = chartPosition.y
        posCanvas.requestPaint()

    }


    Connections {
        target: dm
        /*
        function onSendEigenvaluesJson (s , channelId_ , type){
            if( !s || type !== leq_chart.type ){
                return;
            }
            if( leq_chart.channelId === channelId_ ){
                chart.dataArr = [];

                leq_chart.adding = true
                leq_chart.doAddData( s );
                leq_chart.adding = false;
                console.log( s );
            }
        }
        */

        function onReprocessEv(){
            if( leq_chart.disabled ) return;
            leq_chart.reprocess = true;
            leq_chart.changeAxisType( "x" );
            // dm.getEigenvaluesByType( leq_chart.channelId , leq_chart.type );
        }

        function onReprocessEvFinish(){

            if( leq_chart.disabled ) return;
            leq_chart.reprocess = true;
            leq_chart.changeAxisType( "x" );
            dm.getEigenvaluesByType( leq_chart.channelId , leq_chart.type );
        }

        function onSendEigenvaluesJson (s , channelId_ , type){
            // 切换历史通道数据

            if( leq_chart.disabled ) return;

            if( !s || type !== leq_chart.type ){
                return;
            }

            if( leq_chart.channelId === channelId_ ){

                chart.dataArr = [];
                chart.ys = [];
                chart.ysMax = [];
                var data_ = JSON.parse(s);

                if( leq_chart.isStop && leq_chart.sourseType != "sound" ){
                    var endTime = new Date( data_[data_.length-1]["baseTime"] ).getTime();
                    var startTime = new Date( data_[0]["baseTime"] ).getTime();
                    leq_chart.seconds = Math.ceil(( endTime - startTime ) / 1000 )
                }

                leq_chart.adding = true
                for( var i = 0; i < data_.length; i++ ){
                    /*
                    if( chart.curddate - leq_chart.seconds * 1000 > new Date( data_[i].baseTime ) ){
                        continue;
                    }
                    */
                    leq_chart.doAddData( JSON.stringify(data_[i]) );
                }
                leq_chart.adding = false;
            }
        }

        function onEigenvalueChanged (channelId_ , s){
            if( leq_chart.type == "Level_A" || (leq_chart.sourseType != "sound" && root.isSound) ){
                return;
            }

            if( leq_chart.disabled ) return;
            // 实时添加特征值

            if( !s || leq_chart.adding ){
                return;
            }
            if( leq_chart.channelId === channelId_ ){
                var data_ = JSON.parse(s);
                if( leq_chart.lastTime === data_.baseTime ){
                    return;
                }
                leq_chart.lastTime = data_.baseTime;
                leq_chart.doAddData( s );
            }
        }

        function onSendEigenvalueByType( channelId_ , type_ , ev_ , dateTime , dt , frameCount ){
            if( type_ == "Level_A" || leq_chart.disabled ) return;
            //打开文件切换特征值
            if( channelId_ === leq_chart.channelId && type_ === leq_chart.type ){

                ev_ = JSON.parse(ev_);
                ev_ = ev_[type_];

                if( 0 && !leq_chart.reprocess ){
                    // 时间轴
                    if( Math.round(dt * 1000) != chart.times ){
                        chart.times = Math.round(dt * 1000);
                        chart.x_unit = Math.floor( chart.times/chart.unit );
                        //chart.seconds = dt * ev_.length;
                        chart.count_max = chart.seconds * Math.floor( 1000 / chart.x_unit ) + 1
                    }

                    chart.curddate = new Date(dateTime);

                    if( ev_.length > 0 ){
                        leq_chart.addData( ev_ );
                    }else{
                        chart.dataArr = [];
                        line.clear();
                    }

                }else{
                   // xy轴

                    leq_chart.xmin = leq_chart.xmax - ( ev_.length * dt )
                    // chart.count_max = ev_.length;
                    chart.x_unit = dt;
                    leq_chart.seconds = chart.seconds = dt * ev_.length;
                    chart.count_max = leq_chart.seconds / chart.x_unit + 1;

                    if( ev_.length > 0 ){
                        leq_chart.addXYData( ev_ );
                    }else{
                        chart.dataArr = [];
                        rline.clear();
                    }
                }

                if( leq_chart.reprocess == true ){
                    leq_chart.reprocessFinish();
                }

            }
        }

        function onSendFinalEV( channelId , s , timeLen ){;

            if( leq_chart.disabled ) return;
            if( channelId !== leq_chart.channelId ){
                return;
            }

            chart.dataArr = [];
            chart.ys = [];
            line.clear();

            dm.getEigenvalueData( leq_chart.channelId , leq_chart.type )

            /*
            leq_chart.seconds = Math.ceil( timeLen / 1000 );
            var json = JSON.parse(s);
            for( var d in json ){
                leq_chart.doAddData( JSON.stringify(json[d]) );
            }
            */
        }
    }

    Rectangle{
        anchors.fill: parent
        color: "#CCCCCC"
        z:3
        visible: leq_chart.disabled

        Text{
            text: qsTr("特征值未开通")
            anchors.centerIn: parent
            font.pixelSize: 20 / root.dpi
        }
    }

    ChartView {
        id: chart
        anchors.fill: parent
        anchors.margins: 0 / root.dpi
        antialiasing: false
        theme:ChartView.ChartThemeBlueNcs
        legend.visible : false
        margins.left: 0
        margins.right: 0
        margins.top: 0
        margins.bottom: 0

        property real lablefont: 10/root.dpi
        property date curddate: new Date()
        property string startdate: new Date()
        property int unit: 1 // 单位时间的点数
        property real seconds: leq_chart.seconds // 坐标轴长度60s
        property int times: 200 // 刷新频率 ms
        property real x_unit: Math.floor( chart.times / chart.unit )
        property int count_max: chart.seconds * chart.unit * Math.floor(1000 / chart.times) + 1

        property var dataArr:[]
        property var ys:[]
        property var ysMax:[]

        DateTimeAxis {
            id : axisDate
            min :Date.fromLocaleString(Qt.locale(), chart.startdate, "yyyy-MM-dd hh:mm:ss.zzz")
            max :Date.fromLocaleString(Qt.locale(), chart.startdate, "yyyy-MM-dd hh:mm:ss.zzz")
            format: "ss" //设置显样式
            tickCount : 11
            labelsFont.pixelSize:chart.lablefont
            visible: false
            labelsAngle: 270
        }

        ValueAxis{
            id: axisX
            min: leq_chart.xmin
            max: leq_chart.xmax
            tickCount : 11
            labelsFont.pixelSize: chart.lablefont
            visible: true
            labelsAngle: 270
        }

        Text{
            text: qsTr("时间（s）")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2 / root.dpi
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: chart.lablefont
        }

        ValueAxis{
            id: axisY
            min: leq_chart.ymin
            max: leq_chart.ymax
            labelFormat: "%0.2f" //设置标签的显示精度 样式
            labelsFont.pixelSize: chart.lablefont
            // titleVisible: leq_chart.unity == "" ? true:false
        }

        Text{
            text: leq_chart.unity
            anchors.left: parent.left
            anchors.leftMargin: 5/ root.dpi
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: chart.lablefont
            rotation: -90
            transformOrigin:Item.Left
        }


        LineSeries
        {
            id : line;
            axisX: axisDate
            axisY: axisY
            color: UI_color[UI_config[root.usedConfig].colorType].normal_chart
            useOpenGL: root.useOpengl
            visible: false
            width: 1.0/root.dpi
        }

        LineSeries {
            id : rline;
            axisX: axisX
            axisY: axisY
            color: UI_color[UI_config[root.usedConfig].colorType].normal_chart
            useOpenGL: root.useOpengl
            visible: true
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
                leq_chart.linePos = x;

                var point_ = "";
                if( axisX.visible ){
                    point_ = chart.mapToValue(Qt.point(leq_chart.linePos,1),rline)//使用mapToValue获得该鼠标位置处AreaSeries横坐标的值
                }else{
                    point_ = chart.mapToValue(Qt.point(leq_chart.linePos,1),line)//使用mapToValue获得该鼠标位置处AreaSeries横坐标的值
                }

                if( chart.dataArr.length > 0 ){
                    if( point_.x < chart.dataArr[0].x || point_.x > chart.dataArr[chart.dataArr.length-1].x ){
                         posCanvas.reset()
                         return;
                    }
                }

                var index = -1
                if( axisX.visible ){
                    index = chart.dataArr.findIndex(item => Math.floor( item.x * 10 ) == Math.floor( point_.x * 10) )
                }else{
                    index = chart.dataArr.findIndex(item => Math.floor(item.x / 100) == Math.floor(point_.x/100) )
                }

                if( index === -1 ){
                    return;
                }
                leq_chart.currentIndex = index
                leq_chart.drawTimeLine();
            }

        }
    }

    MouseArea{
        id: mouse_
        anchors.fill: chart
        hoverEnabled: true
        propagateComposedEvents: true
        acceptedButtons: Qt.AllButtons
        cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
        property bool isMove: false
        property real x_: 0

        onPositionChanged: {//获取鼠标移动时的相对位置
            posCanvas.setLinePos( mouse.x )
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
            }
            mouse_.x_ = mouseX
        }

        onClicked:{
            if (mouse.button === Qt.RightButton) {
                option_menu.popup()
            }
        }

        onPressAndHold:{
            if( !mouse_.isMove ){
                option_menu.popup()
            }
        }

        onReleased: {
            mouse_.isMove = false;
            posCanvas.reset();
        }

        onExited:{ //检测鼠标是否离开图表区域
            posCanvas.reset();
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
                    if (dm.saveEvToCSV("eigenvalue",leq_chart.channelId,root.channelName[leq_chart.channelId],"特征值" , leq_chart.evType, dataContent.file!="")){
                        messageDialog.show("保存成功");
                    }else{
                        messageDialog.show("保存失败");
                    }
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

    FolderDialog {
         id: folderDialog
         currentFolder: ""
         acceptLabel: "确定"
         rejectLabel: "取消"
         options: FolderDialog.ShowDirsOnly
         folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
         onAccepted: {
            dm.exportEvToCSV(folderDialog.currentFolder , leq_chart.channelId, root.channelName[leq_chart.channelId] , "特征值" , leq_chart.evType, dataContent.file!="")
         }
    }

    Timer{
        id: timer
        interval: chart.times
        repeat: true
        onTriggered: {
            leq_chart.getData()
        }
    }

    function start(){

        chart.dataArr = [];
        chart.ys = [];
        chart.ysMax = [];
        leq_chart.initChart()
        leq_chart.isStop = false;
        posCanvas.reset();
        // leq_chart.getData()
        // 开启计时器
        // timer.start()
    }

    function stop(){

        leq_chart.isStop = true;
        // timer.stop()
        // leq_chart.initChart()
    }

    function initChart(){

        if( leq_chart.disabled ) return;
        leq_chart.seconds = 10
        var curd = chart.curddate;
        var mintimestr = curd.toLocaleString(Qt.locale(),"yyyy-MM-dd hh:mm:ss.000");
        curd.setSeconds(curd.getSeconds()+chart.seconds);
        var maxtimestr = curd.toLocaleString(Qt.locale(),"yyyy-MM-dd hh:mm:ss.000");
        axisDate.min =  Date.fromLocaleString(Qt.locale(), mintimestr, "yyyy-MM-dd hh:mm:ss.000")
        axisDate.max =  Date.fromLocaleString(Qt.locale(), maxtimestr, "yyyy-MM-dd hh:mm:ss.000")
        axisX.min = -leq_chart.seconds
        axisX.max = 0
        line.clear();
        rline.clear()
    }

    // 打开历史文件
    function setData( data ){

        if( leq_chart.disabled ) return;
        var max = axisY.max
        var max_ = 0

        line.clear();
        rline.clear();
        var curd = chart.curddate;
        var timeline = curd.getTime()
        var isFull = false;

    }

    // 添加相对时间轴数据
    function addXYData( data ){

        if( leq_chart.disabled ) return;
        var max = axisY.max
        var max_ = 0

        chart.dataArr = [];
        rline.clear()

        let isFull = false;
        let max_once = -Infinity;
        let isOut = false;
        for (var i = 0; i < data.length; ++i)
        {
            // chart.dataArr.push({x: leq_chart.xmin + chart.x_unit * i , y:data[i]})
            chart.ys.push( data[i] )
            if( chart.ys.length - 1 > chart.count_max ){
                // chart.dataArr.shift()
                chart.ys.shift()
                isOut = true;
            }
            if( data[i] > max_once ) max_once = data[i];
        }
        chart.ysMax.push( max_once );

        if( isOut && chart.ysMax.length > 1 ){
            chart.ysMax.shift();
        }

        // if( chart.dataArr.length > 0 ){
        if( chart.ys.length > 0 ){
            axisX.min = leq_chart.xmin

            max_ = Math.max(...chart.ysMax);

            // max_ = chart.ys.reduce((a, b) => Math.max(a, b), -Infinity)
            if( ( max_ * 10 < max || max_ > max ) && max_ != 0 ){
                //axisY.max = max_
                if( Math.abs(max_) > 1 ){
                    axisY.max = max_ * 1.5;
                    axisY.labelFormat = "%d"
                }else{
                    var k = 0;
                    while( Math.abs(max_) < 1 ){
                        max_ = max_ * 10;
                        k++;
                    }
                    axisY.max = (Math.ceil(max_) + 1) * Math.pow(10,-k);
                    axisY.labelFormat = "%0."+k+"f";
                }
            }

            i = 0;
            for( var n = chart.ys.length-1; n >= 0; n-- ){
                rline.append( leq_chart.xmax - chart.x_unit * i , chart.ys[n] )
                chart.dataArr.unshift({x: leq_chart.xmax - chart.x_unit * i , y:chart.ys[n]})
                i++;
            }
        }
    }

    // 添加绝对时间轴数据
    function addData( data ){
        if( leq_chart.disabled ) return;

        var max = axisY.max
        var max_ = 0

        line.clear()

        var curd = chart.curddate;
        var timeline = curd.getTime()

        var isFull = false;

        for (var i = 0; i < data.length; ++i)
        {
            chart.dataArr.push({x:timeline,y:data[i]})
            chart.ys.push( data[i] )
            if( ( chart.dataArr[0].x + chart.seconds * 1000 ) < timeline ){
            //if(  chart.dataArr.length > chart.count_max ){
                chart.dataArr.shift()
                chart.ys.shift()
                isFull = true
            }

            /*
            if( data[i] > max  ){
                max_ = data[i];
            }
            */

            timeline += chart.x_unit
        }

        if( chart.dataArr.length > 0 ){
            var mintimestr = new Date(chart.dataArr[0].x)
            var maxtimestr = new Date(chart.dataArr[0].x + chart.seconds * 1000 )

            mintimestr = mintimestr.toLocaleString(Qt.locale(),"yyyy-MM-dd hh:mm:ss.zzz")
            maxtimestr = maxtimestr.toLocaleString(Qt.locale(),"yyyy-MM-dd hh:mm:ss.zzz")
            axisDate.min =  Date.fromLocaleString(Qt.locale(), mintimestr, "yyyy-MM-dd hh:mm:ss.zzz")
            axisDate.max =  Date.fromLocaleString(Qt.locale(), maxtimestr, "yyyy-MM-dd hh:mm:ss.zzz")

            max_ = Math.max(...chart.ys)<1?1:Math.max(...chart.ys);
            /*
            if( max_ != max ){
               axisY.max = max_ + max_*0.05
            }*/
            max_ = Math.max(...chart.ys);

            if( ( max_ * 10 < max ||  max_ > max ) && max_ != 0 ){
                //axisY.max = max_
                if( Math.abs(max_) > 1 ){
                    axisY.max = max_ + 1;
                    axisY.labelFormat = "%d"
                }else{
                    var k = 0;
                    while( Math.abs(max_) < 1 ){
                        max_ = max_ * 10;
                        k++;
                    }
                    axisY.max = (Math.ceil(max_) + 1) * Math.pow(10,-k);
                    axisY.labelFormat = "%0."+k+"f"
                }
            }

            for( var n = 0; n < chart.dataArr.length; n++ ){
                line.append( chart.dataArr[n].x , chart.dataArr[n].y )
            }
        }
    }

    // test 生成随机数据
    function getData(){
        if( leq_chart.disabled ) return;
        line.clear()
        var curd = new Date();

        var t = Math.floor( Math.floor(chart.dataArr.length / chart.unit ) / ( 1000 / chart.times ) )
        var mintimestr
        var maxtimestr
        if( t >= chart.seconds ){
            curd.setSeconds(curd.getSeconds() - t );
            mintimestr = curd.toLocaleString(Qt.locale(),"yyyy-MM-dd hh:mm:ss.zzz");
            curd.setSeconds(curd.getSeconds() + chart.seconds );
            maxtimestr = curd.toLocaleString(Qt.locale(),"yyyy-MM-dd hh:mm:ss.zzz");
        }

        var nowTime = new Date().getTime();
        var x_unit = Math.floor( chart.times/chart.unit);

        var count_max = chart.seconds * chart.unit * Math.floor(1000 / chart.times) + 1

        for (var i = 0; i < chart.unit; ++i)
        {

            if( chart.dataArr.length > count_max ){
                //line.removePoints(0,chart.unit)
                chart.dataArr.shift()
            }
           // console.log( chart.dataArr.length )
            var timeline = nowTime + x_unit * i;
            chart.dataArr.push({x:timeline,y:Math.random()*100})
            //line.append(timeline ,Math.random()*100);
        }

        if( t >= chart.seconds ){
            axisDate.min =  Date.fromLocaleString(Qt.locale(), mintimestr , "yyyy-MM-dd hh:mm:ss.zzz")
            axisDate.max =  Date.fromLocaleString(Qt.locale(), maxtimestr, "yyyy-MM-dd hh:mm:ss.zzz")
        }

        for( var n = 0; n < chart.dataArr.length; n++ ){
            line.append( chart.dataArr[n].x ,  chart.dataArr[n].y )
        }
    }

    Component.onCompleted:{
        leq_chart.initChart();
    }

}
