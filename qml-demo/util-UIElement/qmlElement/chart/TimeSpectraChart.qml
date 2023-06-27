import QtQuick 2.15
import TimeSpectra 1.0
import GradientBar 1.0
import TimerCursor 1.0
import InstantSpectrum 1.0

Item {
    id: chart
    anchors.fill: parent

    signal timeLineChanged( int timeline , int currentFrame )
    signal changeTimerIntval( int ms )
    signal finish()
    signal currentFrameChanged( int currentFrame )
    signal frequencyChanged( int frequencyMin , int frequencyMax )

    property int method: 0
    property int calCount: 4

    property bool isSpectraFinish: false
    property bool isSpectrumFinish: false
    property real currentTimeLine: 0

    property bool isFull: false
    property string file: ""

    property string channelId: "sound"
    property string sourseType: "sound"

    //property int seconds: chart.sourseType=="sound"?dm.getSoundRecordTime()/1000:dm.getVibrationRecordTime()/1000
    property int seconds: 10

    property bool isChangeChannel: false

    property bool ln:false
    property bool adaption:false

    property bool isReset: false

    property string mode: cm.getParamConfig("soundcam.storage/workingState")!==""?cm.getParamConfig("soundcam.storage/workingState"): "Ultra" // Standard, Ultra  // 默认Standard

    property string canvasFont: Math.floor(10/root.dpi).toString() + "px '"+ root.family +"'"

    property double bandWidth: chart.sourseType == "sound"? ( chart.mode=="Ultra"?100:25 ) :4 // khz
    property double df: 1
    property double yunit:chart.sourseType == "sound"? ( chart.mode=="Ultra"?20:5 ) :0.8 // khz

    property double dt: 0.000005
    property int times: 0

    onModeChanged: {
        spectrumXaisY.requestPaint();
        frequencyLine.requestPaint();
    }

    function setcalCount( v ){
        if( v <= 500 ){
            chart.method = 0;
            chart.calCount = 1;
        }else if( v <= 1000 ){
            chart.method = 0;
            chart.calCount = 2;
        }else if( v == 2500 ){
            chart.method = 0;
            chart.calCount = 5;
        }else if( v == 5000 ){
            chart.method = 0;
            chart.calCount = 10;
        }else if( v <= 3000 ){
            chart.method = 0;
            chart.calCount = 4;
        }else{
            chart.method = 0;
            chart.calCount = 8;
        }

    }

    Connections {
        target: dm

        function onSendFinalSpectra( channelId , d ){
            if( chart.sourseType === "sound" ){
                spectrumChart.setFinalSpectraData( d )
                spectraChart.setFinalSpectraData( d )
            }
        }

        function onSendFinalSpectraLocal( channelId , d ){
            if( chart.sourseType === "sound" ){
                spectrumChart.setFinalSpectraLocalData( d )
            }
        }

        function onSpectraChanged( channelId_ , d){
            //测试
            if( channelId_ === chart.channelId ){
                spectraChart.addData( d )
                if( !root.isHide && root.isSound ){
                    spectrumChart.addData( d )
                }
            }
        }

        function onSpectraLocalChanged( channelId_ , d ){
            if( channelId_ === chart.channelId && !root.isHide && root.isSound  ){
                spectrumChart.addLocalData( d )
            }
        }

        // 实时切换通道
        function onSendSpectra( data , channelId_ ){

            if( channelId_ === chart.channelId ){
                isChangeChannel = true;

                if( data_vibration.isStop ){
                    var json_ = JSON.parse( data[0] );
                    spectraChart.frameWidth = json_["frameCount"] * data.length;
                    spectraChart.resetSpectra();

                    chart.seconds = json_["frameCount"] * data.length / json_["deltaFreq"]
                    spectraXais.timeLen = chart.seconds * 1000
                    spectraXais.requestPaint()

                }

                for( var i = 0; i < data.length; i++ ){
                    spectraChart.addJsonData( JSON.parse(data[i]) )
                }
                spectrumChart.addJsonData( JSON.parse(data[data.length-1]) )
                isChangeChannel = false
            }
        }

        function onSpectraJsonChanged( channelId_ , json_ ){
            if( ( channelId_ === chart.channelId && !chart.isChangeChannel ) ){
                // var json_ = JSON.parse(d);
                if( chart.bandWidth*1000 != json_["bandWidth"] || chart.df != json_["deltaFreq"] || chart.times == 0 ){

                    chart.bandWidth = json_["bandWidth"] / 1000;
                    chart.df = json_["deltaFreq"];
                    chart.yunit = chart.bandWidth / 5;

                    chart.setcalCount( json_["bandWidth"] / json_["deltaFreq"] )

                    spectrumChart.yAxisMax = chart.bandWidth;

                    spectrumChart.yAxisUnit = chart.yunit;

                    spectrumChart.frameLen = json_["bandWidth"] / json_["deltaFreq"];
                    spectrumXaisY.requestPaint();

                    spectraChart.frameWidth = json_["deltaFreq"] * chart.seconds;
                    spectraChart.frameHeight =  json_["bandWidth"] / json_["deltaFreq"] * 1.28;
                    spectraChart.resetSpectra();
                    chart.times++;
                }

                if( root.isVibration ){
                    spectraChart.addJsonData( json_ )
                }
                if( !root.isHide && root.isVibration ){
                    spectrumChart.addJsonData( json_ )
                }
            }
        }

        function onSendSpectraFromFile( channelId_ , json_ , len ){
            // 获取振动历史文件多通道数据
            if( channelId_ == chart.channelId ){
                // var json_ = JSON.parse(data_);

                if( !json_["bandWidth"] ){
                    return;
                }

                if( chart.bandWidth*1000 != json_["bandWidth"] || chart.df != json_["deltaFreq"] || chart.times == 0 ){

                    chart.bandWidth = json_["bandWidth"] / 1000
                    chart.df = json_["deltaFreq"];
                    chart.yunit = chart.bandWidth / 5

                    chart.setcalCount(  json_["bandWidth"] / json_["deltaFreq"] )

                    spectrumChart.yAxisMax = chart.bandWidth;
                    spectrumChart.yAxisUnit = chart.yunit;
                    spectrumChart.frameLen = json_["bandWidth"] / json_["deltaFreq"];
                    spectrumXaisY.requestPaint();

                    spectraChart.frameWidth = json_["deltaFreq"] * chart.seconds;
                    spectraChart.frameHeight =  json_["bandWidth"] / json_["deltaFreq"] * 1.28;
                    spectraChart.resetSpectra();

                }
                spectraChart.addJsonData( json_ )
                spectrumChart.addJsonData( json_ )
            }
        }
    }

    onIsFullChanged: {
        timerCursor.fullChanged()
        chart.reset();
    }
    // 设置frequency控件参数

    function reset(){
        chart.isReset = true

        var width_ = spectraChart.width
        spectraChart.height = spectraChartRect.height;
        spectraChart.width = spectraChartRect.width;
        spectraChart.x = 0;
        spectraChart.y = 0;

        spectrumRect.height = spectrumChartRect.height;
        spectrumRect.width = spectrumChartRect.width;
        spectrumRect.y = 0

        spectraXais.x = 0;
        spectraXais.count = spectraXaisRect.count
        spectraXais.requestPaint();

        spectrumXaisY.y = 0;
        spectrumXaisY.unit_count = spectrumXaisYRect.count
        spectrumXaisY.requestPaint();

        var timelineX = timerCursor.timelineX;
        timerCursor.setPostion( timelineX / width_ * spectraChart.width );

        spectrumChart.yAxisUnit = spectrumRect.yAxisUnit

        chart.isReset = false
    }

    function move( x1 , y1 , x , y , unit = 5 ){
        var moveX = 0
        var moveY = 0
        if( x > x1 ){ // x右移
            moveX = spectraChart.x>=0?0:unit
        }else if( x < x1 ){ // x左移
            moveX = spectraChart.x + spectraChart.width <= spectraChartRect.width?0:-unit
        }
        spectraChart.x = spectraChart.x + moveX;

        if( y > y1 ){ // y下移
            moveY = spectraChart.y>=0?0:unit
        }else if( y < y1 ){ // y上移
            moveY = spectraChart.y + spectraChart.height <= spectraChartRect.height?0:-unit
        }
        spectraChart.y = spectraChart.y + moveY;
        chart.spectraCheckRange()

        //移动切片
        /*
        if( y > y1 ){ // y下移
            moveY = spectrumRect.y>=0?0:5
        }else if( y < y1 ){ // y上移
            moveY = spectrumRect.y + spectrumRect.height <= spectrumChartRect.height?0:-5
        }
        */
        spectrumRect.y = spectrumRect.y + moveY;
        chart.spectrumCheckRange()

        // spectra x坐标
        /*
        if( x > x1 ){ // x右移
            moveX = spectraChart.x>=0?0:5
        }else if( x < x1 ){ // x左移
            moveX = spectraChart.x + spectraChart.width <= spectraChartRect.width?0:-5
        }
        */
        spectraXais.x = spectraXais.x + moveX;
        chart.spectraXaisCheckRange()

        spectrumXaisY.y = spectrumXaisY.y + moveY;
        chart.spectrumXaisYCheckRange()

    }

    function resize( x , y , act ){
       // 放大缩小
       if( x < 0 ){
            x = 0
       }
       if( x > timerCursor.width ){
            x = timerCursor.width
       }

//        console.log("x:" + x );
//        console.log("y:" + y );
       var width_ = spectraChart.width;
       var height_ = spectraChart.height;
       var height2_ = spectrumRect.height;
       var width_x = spectraXais.width;
       var height_y = spectrumXaisY.height;

       if( act === "enlarge" ){
           spectraChart.width = spectraChart.width*2 > spectraChartRect.width*16?spectraChartRect.width*16:spectraChart.width*2;
           spectraChart.height = spectraChart.height*2 > spectraChartRect.height*16?spectraChartRect.height*16:spectraChart.height*2;

           //切片画图坐标轴
           spectrumChart.yAxisUnit = spectrumChart.yAxisUnit/2 < spectrumRect.yAxisUnit/16?spectrumRect.yAxisUnit/16:spectrumChart.yAxisUnit/2;

           //切片放大 y轴
           spectrumRect.height = spectrumRect.height*2 > spectrumChartRect.height*16? spectrumChartRect.height*16: spectrumRect.height*2;

           //spectra 坐标轴放大 x轴
           spectraXais.count = spectraXais.count*2 > spectraXaisRect.count*16?spectraXaisRect.count*16:spectraXais.count*2;

           //spectra 坐标轴放大 y轴
           spectrumXaisY.unit_count = spectrumXaisY.unit_count*2 > spectrumXaisYRect.count*16?spectrumXaisYRect.count*16:spectrumXaisY.unit_count*2;

           // spectrumChart.yAxisUnit = spectrumChart.yAxisUnit*2;
       }else{
           spectraChart.width = spectraChart.width/2 < spectraChartRect.width?spectraChartRect.width:spectraChart.width/2;
           spectraChart.height = spectraChart.height/2 < spectraChartRect.height?spectraChartRect.height:spectraChart.height/2;

           //切片画图坐标轴
           spectrumChart.yAxisUnit = spectrumChart.yAxisUnit*2 > spectrumRect.yAxisUnit?spectrumRect.yAxisUnit:spectrumChart.yAxisUnit*2;

           // 切片缩小
           spectrumRect.height = spectrumRect.height/2 < spectrumChartRect.height? spectrumChartRect.height: spectrumRect.height/2;

           //spectra 坐标轴缩小 x轴
           spectraXais.count = spectraXais.count/2 < spectraXaisRect.count?spectraXaisRect.count:spectraXais.count/2;

           //spectra 坐标轴缩小 y轴
           spectrumXaisY.unit_count = spectrumXaisY.unit_count/2 < spectrumXaisYRect.count?spectrumXaisYRect.count:spectrumXaisY.unit_count/2;

           // spectrumChart.yAxisUnit = spectrumChart.yAxisUnit/2;
       }

       spectraChart.x = -( x - spectraChart.x )/width_ * spectraChart.width + x
       spectraChart.y = -( y - spectraChart.y )/height_ *spectraChart.height + y
       chart.spectraCheckRange();

       // 切片
       spectrumRect.y = -( y - spectrumRect.y )/height2_ *spectrumRect.height + y
       chart.spectrumCheckRange();

       //坐标轴
       spectraXais.x = -( x - spectraXais.x )/width_x * spectraXais.width + x
       spectraXais.requestPaint();
       chart.spectraXaisCheckRange();

       spectrumXaisY.y = -( y - spectrumXaisY.y )/height_y * spectrumXaisY.height + y
       spectrumXaisY.requestPaint();
       chart.spectrumXaisYCheckRange();

       // 重新计算所选时间轴的位置
       var timelineX = timerCursor.timelineX;
       timerCursor.setPostion( timelineX / width_ * spectraChart.width );
    }

    function spectraCheckRange(){
        if( spectraChart.x + spectraChart.width < spectraChartRect.width ){
            spectraChart.x = spectraChartRect.width - spectraChart.width;
        }
        if( spectraChart.y + spectraChart.height < spectraChartRect.height ){
            spectraChart.y = spectraChartRect.height - spectraChart.height;
        }
        // 边界检测
        if( spectraChart.x > 0 || spectraChart.x + spectraChart.width < spectraChartRect.width ){
            spectraChart.x = 0
        }
        if( spectraChart.y > 0 || spectraChart.y + spectraChart.height < spectraChartRect.height ){
            spectraChart.y = 0
        }
    }

    function spectrumCheckRange(){
        if( spectrumRect.y + spectrumRect.height < spectrumChartRect.height ){
            spectrumRect.y = spectrumChartRect.height - spectrumRect.height;
        }
        // 边界检测
        if( spectrumRect.y > 0 || spectrumRect.y + spectrumRect.height < spectrumChartRect.height ){
            spectrumRect.y = 0
        }
    }

    function spectraXaisCheckRange(){
        if( spectraXais.x + spectraXais.width < spectraXaisRect.width ){
            spectraXais.x = spectraXaisRect.width - spectraXais.width;
        }
        // 边界检测
        if( spectraXais.x > 0 || spectraXais.x + spectraXais.width < spectraXaisRect.width ){
            spectraXais.x = 0
        }
    }

    function spectrumXaisYCheckRange(){
        if( spectrumXaisY.y + spectrumXaisY.height < spectrumXaisYRect.height ){
            spectrumXaisY.y = spectrumXaisYRect.height - spectrumXaisY.height;
        }
        // 边界检测
        if( spectrumXaisY.y > 0 || spectrumXaisY.y + spectrumXaisY.height < spectrumXaisYRect.height ){
            spectrumXaisY.y = 0
        }
    }

    function setChannel( channelId_ ){
        if( 1 || channelId_ !== chart.channelId ){
            chart.channelId = channelId_

            spectraChart.resetSpectra();

            if( chart.file != "" ){
                dm.getSpectraFromFile( chart.channelId )
            }else{
                dm.getSpectraData( chart.channelId )
            }
        }
    }

    // 打开保存文件 切换特征值
    function changeType( type ){
        if( leq_chart.type !== type ){
            leq_chart.type = type
            dm.getSpectraFromFile( leq_chart.channelId );
        }
    }

    function getDataFromOpenData(){
        dm.getSpectraFromFile( chart.channelId )
    }

    function setData( data , bandWidth ){
        /*
        if( dt_ !== chart.dt ){
            chart.dt = dt_
            if( dt_ !== 0.000005 ){
                chart.mode = "Standard"
                chart.yunit = 5
            }

            chart.bandWidth = Math.ceil(1/chart.dt/2/1000)
            timerCursor.frequency = chart.bandWidth
            spectrumXaisY.requestPaint();
        }
        */

        console.log( bandWidth + "bandWidth" );

        if( bandWidth / 1000 <= 25 ){
            chart.mode = "Standard"
            chart.yunit = 5
        }

        chart.bandWidth = bandWidth / 1000
        timerCursor.frequency = chart.bandWidth
        spectrumXaisY.requestPaint();

        spectrumChart.setData(data["spectrum"])
        spectraChart.setData(data["spectra"])
    }

    function checkFinish(){
        if( isSpectraFinish && isSpectrumFinish ){
            chart.finish();
        }
    }

    function setTimeCursor( channelId_ , count , time , timeList ){
        if( channelId_ === chart.channelId ){

            chart.seconds = time / 1000;
            timerCursor.setFrameCount( count , time , timeList )
            timerCursor.setDuration( time )

            timerCursor.visible = true;

            // chart.changeTimerIntval( time / count )
            spectraXais.timeLen = time;
            spectraXais.requestPaint();
        }
    }

    function setTimeCursorCurrent( currentFrame ){
       timerCursor.setCurrentFrame( currentFrame )
    }

    function setAmplitude( min , max ){
        if( chart.file != "" ){
            spectraChart.setFrequence(max , min , false);
            spectraChart.clearData();
        }else{
            spectraChart.setFrequence(max , min , true);
        }

        chartGradientBar.maxDB = max;
        chartGradientBar.minDB = min;
        chartGradientBar.repaint();
    }

    Timer{
        id: timer
        running:false
        function setTimeout(cb, delayTime) {
            timer.interval = delayTime;
            timer.repeat = false;
            timer.triggered.connect(cb);
            timer.triggered.connect(function release () {
                timer.triggered.disconnect(cb); // This is important
                timer.triggered.disconnect(release); // This is important as well
            });
            timer.start();
        }
    }

    function hideFrequency(){
        timer.setTimeout( function(){
            timerCursor.visible = false;
        } , 1000)
    }

    function setFrequencyMax( frequencyMax ){
        timerCursor.setFrequencyMax( frequencyMax )
        timerCursor.visible = true;
        frequencyLine.requestPaint()
    }

    function setFrequencyMin( frequencyMin ){
        timerCursor.setFrequencyMin( frequencyMin )
        timerCursor.visible = true;
        frequencyLine.requestPaint()
    }

    function mouseClick( x , y ){

        if( x < 0 ){
            x = 0
        }
        if( x > timerCursor.width ){
            x = timerCursor.width
        }

        timerCursor.setPostion( x - spectraChart.x );
        // chart.timeLineChanged( timerCursor.getClickTimeLine() );
        // spectrumChart.timeLineChanged( spectraChart.getClickSize(x) );

    }

    onFileChanged: {
        if( chart.file != "" ){
            // spectraChart.openFile(file);
            // spectrumChart.openFile(file);
            timerCursor.visible = false;
        }else{
            timerCursor.visible = false;
        }
    }

    /*
    function showNext(){
        var x = timerCursor.changeTimeCursor();
        chart.timeLineChanged( timerCursor.getClickTimeLine() );
        spectrumChart.timeLineChanged();
    }
    */

    function play(){
        timerCursor.play();
    }
    function stopPlay(){
        timerCursor.stop();
    }

    function start(){

        chart.seconds = 10

        spectraXais.timeLen = chart.seconds * 1000
        spectraXais.requestPaint()

        timerCursor.resetTimerCursor();

        if( chart.channelId == "sound" ){
            spectraChart.frameWidth = 24.6 * chart.seconds;
        }else{
            spectraChart.frameWidth = chart.df * chart.seconds;
        }

        spectraChart.start();
        spectrumChart.resetCurveLine();
    }

    function stop(){
        spectraChart.stop();
    }

    Rectangle{
        id: spectraChartRect
        anchors.left: parent.left
        anchors.top: parent.top
        width: ( parent.width ) * 0.7
        height: parent.height
        clip: true

        TimeSpectra {
            id: spectraChart
            x:0
            y:0
            width: parent.width
            height: parent.height
            //anchors.left: parent.left
            //anchors.top: parent.top
            //width: ( parent.width ) * 0.7
            //height: parent.height
            //frameWidth: chart.channelId == "sound"?100:100
            frameWidth:chart.channelId == "sound"? 24.6 * chart.seconds: chart.df * chart.seconds ;
            frameHeight:chart.channelId == "sound"? 1024: chart.bandWidth / chart.df;
            method: chart.method
            calCount: chart.calCount
            amplitudeMax: chart.sourseType =="sound"?80:5
            ln:chart.ln
            adaption: chart.adaption

            Component.onCompleted: {
                spectraChart.resetSpectra();
            }

            // full: chart.isFull
            onFrameCountChanged: {
                chart.frequencyChanged(getFrequencyMin() , getFrequencyMax()) // 修改控件的初始参数
                timerCursor.setFrequency( getFrequencyMin() , getFrequencyMax() )
            }
            onFinish: {
                isSpectraFinish = true;
                chart.checkFinish();
            }

            onAmplitudeMaxChanged: {
                if( chartGradientBar.maxDB != spectraChart.amplitudeMax ){
                    chartGradientBar.maxDB = spectraChart.amplitudeMax;
                    chartGradientBar.repaint()
                }
                spectrumXais.len = spectraChart.amplitudeMax / 5
                spectrumXais.requestPaint();

                spectrumChart.xAxisMax = spectraChart.amplitudeMax;
                spectrumChart.xAxisUnit = spectraChart.amplitudeMax / 5;
            }

            TimeAxis{
                id:timerCursor
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width
                height: parent.height
                z: 99999
                fontSize: 10/root.dpi
                visible: false
                frequency: chart.bandWidth
                onHeightChanged: {
                    timerCursor.fullChanged()
                }
                onCurrentFrameChanged: {
                    chart.currentTimeLine = timerCursor.getClickTimeLine();
                    spectrumChart.timeLineChanged( chart.currentTimeLine );
                    chart.timeLineChanged( chart.currentTimeLine , currentFrame );
                    chart.currentFrameChanged( currentFrame );
                }
                onFrameCountChanged: {
                    spectraXais.requestPaint()
                }
            }
        }
    }

    Rectangle{
        id:spectraXaisRect
        anchors.top: spectraChartRect.bottom
        anchors.right: spectraChartRect.right

        width: spectraChartRect.width + 20 / root.dpi
        height: 20/root.dpi
        clip: false
        color: "transparent"
        property real count: 10

        Canvas{
            id: spectraXais
            x: 0
            y: 0
            width: spectraXaisRect.width
            height: parent.height
            z: 99999
            property real drawWidth: spectraChart.width

            property real timeLen: chart.seconds * 1000
            property real count: 10

            onPaint: {
                var ctx = spectraXais.getContext("2d");

                ctx.clearRect(0,0,spectraXais.width,spectraXais.height);
                ctx.beginPath();

                ctx.font = chart.canvasFont
                ctx.fontWeight = "normal"

                ctx.lineWidth=1
                // ctx.fillStyle="red";

                // var len = parseInt( timerCursor.frameCount / 1000) > 10?parseInt( timerCursor.frameCount / 1000):10
                var len = spectraXais.count;

                // var unit = spectraXais.timeLen > 10000? spectraXais.timeLen / ( 1000 * len): 1

                var unit
                if( spectraXais.timeLen < 1000 * len ){
                    unit = Math.ceil(spectraXais.timeLen / (1000 * len));
                }else{
                    unit = Math.round(spectraXais.timeLen / (1000 * len));
                }

                var wUnit = spectraXais.drawWidth / len * ( unit * len / ( spectraXais.timeLen / 1000 ))

                for( var i = 0; i <= len; i++ ){
                    if( ( spectraXais.drawWidth - i*wUnit + 2 ) > 0 ){
                        ctx.moveTo(spectraXais.width - i*wUnit , 0 );
                        ctx.lineTo(spectraXais.width - i*wUnit , 3/root.dpi);

                        ctx.text( i===parseInt( len / 2 )?"时间(s)":(-i*unit).toFixed(0) , i==len/2?spectraXais.width - i*wUnit-18/root.dpi:spectraXais.width - i*wUnit - 8/root.dpi, 10/root.dpi );
                    }
                }
                ctx.strokeStyle = '#333333';
                ctx.stroke();
                ctx.fill();
            }
        }
    }

    ChartGradientBar{
        id: chartGradientBar
        width: 15/root.dpi
        height: parent.height
        anchors.left: spectraChartRect.right
        anchors.top: parent.top
        z:10
        maxDB: chart.sourseType == "sound"?80:5
        minDB: 0
        showRange: false
        fontSize: 10 / root.dpi
    }

    Rectangle{
        id: spectrumChartRect
        width: ( parent.width ) * 0.3 - chartGradientBar.width
        height:  parent.height
        anchors.right: parent.right
        anchors.top: parent.top
        clip:true

        Rectangle{
            id: spectrumRect
            width: parent.width
            height: parent.height
            x:0
            y:0
            clip:true

        //    property double yAxisUnit: chart.sourseType == "sound"?5.0:1.0
            property double yAxisUnit: chart.yunit

            InstantSpectrumChart{
                id: spectrumChart
                // 坐标轴偏移 非5倍数情况下 需要需要位移掉多出的部分

                // property real offset: chart.bandWidth % chart.yunit > 0? spectrumRect.height / chart.bandWidth:0
                property real offset: 0

                width: parent.width + 32
                height: parent.height + 40 + spectrumChart.offset

                frameLen: chart.sourseType == "sound"?1024:2500
                amplitudeMax: chart.sourseType == "sound"?80.0:5.0
                amplitudeMin: 0.0
                frequenceWidth: chart.bandWidth

                xAxisMin: chart.sourseType == "sound"?-20:0.0
                xAxisMax: chart.sourseType == "sound"?100.0:5.0
                yAxisMin: 0.0
                yAxisMax: chart.bandWidth

                xAxisUnit: chart.sourseType == "sound"?20.0:1.0
//                yAxisUnit: chart.sourseType == "sound"?5.0:1.0
                yAxisUnit: chart.yunit

                ln:false
                x: -16
                y: Qt.platform.os=="linux" ? - ( 15 + spectrumChart.offset ) : - ( 15 + spectrumChart.offset )

                onDrawFrequency: {
                    frequencyLine.requestPaint()
                }
                onFinish: {
                    isSpectrumFinish = true;
                    chart.checkFinish();
                }

                Component.onCompleted: {
                    spectrumChart.setRange()
                }

            }

            Canvas{
                id: frequencyLine
                anchors.fill:  parent
                z: 99999

                property int len: chart.bandWidth

                onPaint: {
                    var ctx = frequencyLine.getContext("2d");
                    ctx.clearRect(0,0,frequencyLine.width,frequencyLine.height);

                    ctx.beginPath();
                    ctx.font = chart.canvasFont
                    ctx.lineWidth=1
                    // ctx.fillStyle="red";
                    ctx.moveTo(0 , frequencyLine.getPos( timerCursor.getFrequencyMin() ) );
                    ctx.lineTo(width, frequencyLine.getPos( timerCursor.getFrequencyMin() ) );

                    if( frequencyLine.getPos( timerCursor.getFrequencyMin() ) < 20 ){
                        ctx.text(timerCursor.getFrequencyMin()+" Hz", 10, frequencyLine.getPos( timerCursor.getFrequencyMin() ) + 20 );
                    }else{
                        ctx.text(timerCursor.getFrequencyMin()+" Hz", 10, frequencyLine.getPos( timerCursor.getFrequencyMin() ) - 6 );
                    }
                    ctx.moveTo(0 , frequencyLine.getPos( timerCursor.getFrequencyMax() ) );
                    ctx.lineTo(width, frequencyLine.getPos( timerCursor.getFrequencyMax() ) );

                    if( frequencyLine.getPos( timerCursor.getFrequencyMax() ) < 20 ){
                        ctx.text(timerCursor.getFrequencyMax()+" Hz", 10, frequencyLine.getPos( timerCursor.getFrequencyMax() ) + 20 );
                    }else{
                        ctx.text(timerCursor.getFrequencyMax()+" Hz", 10, frequencyLine.getPos( timerCursor.getFrequencyMax() ) + -6 );
                    }
                    ctx.strokeStyle = '#ffffff';
                    ctx.fillStyle = "#ffffff";
                    ctx.stroke();
                    ctx.fill();
                }

                function getPos( data ){
                    return parseInt( height / frequencyLine.len * (frequencyLine.len - data / 1000) )
                }
            }
        }
    }

    Canvas{
        id: spectrumXais
        anchors.top: spectrumChartRect.bottom
        //anchors.right: spectrumChartRect.right
        anchors.left: chartGradientBar.left

        width: spectrumChartRect.width + chartGradientBar.width + spectrumXais.drawStart
        height: 20/root.dpi
        z: 99999
        property real len: chart.sourseType == "sound"?20:1
        property string text: chart.sourseType == "sound"?"幅值dB(A)":"加速度(g)"

        property real drawWidth: spectrumChartRect.width
        property real drawStart: 20 / root.dpi


        function format(f){
            if( Math.abs(f) > 10 || Math.abs(f) === 0 ){
                f = f.toFixed(0);
            }else if( Math.abs(f) > 1  ){
                f = f.toFixed(1);
            }else {
                if( Math.abs(f) > 0.1 ){
                    f = f.toFixed(1);
                }else if( Math.abs(f) > 0.0001 ) {
                    f = (f*1000).toFixed(1) + "m";
                }else{
                    f = (f*1000000).toFixed(1) + "μ";
                }
            }
            return f;
        }

        onPaint: {
            var ctx = spectrumXais.getContext("2d");
            ctx.clearRect(0,0,spectrumXais.width,spectrumXais.height);

            ctx.beginPath();
            // ctx.fillStyle="red";
            ctx.lineWidth = 1/root.dpi;
            ctx.font = chart.canvasFont
            var len = chart.sourseType=="sound"?6:5
            var min = chart.sourseType=="sound"?-20:0
            var wUnit = spectrumXais.drawWidth / len
            var x = spectrumXais.len

            for( var i = 0; i <= len; i++ ){
                ctx.moveTo(spectrumXais.width - i*wUnit - spectrumXais.drawStart , 0 );
                ctx.lineTo(spectrumXais.width - i*wUnit - spectrumXais.drawStart , 3/root.dpi);

                if( i == 0 ){
                    ctx.text( spectrumXais.format( i*x + min ) , spectrumXais.width - i*wUnit - spectrumXais.drawStart - 10/root.dpi , 10/root.dpi);
                }else if( i == len ){
                    ctx.text( spectrumXais.format( i*x + min ) , spectrumXais.width - i*wUnit - spectrumXais.drawStart - 10/root.dpi , 10/root.dpi);
                }else if( i == parseInt(len / 2) - 1 || i == parseInt(len / 2) + 1 ){

                }else if( i === parseInt(len / 2) ){
                    ctx.text( spectrumXais.text , spectrumXais.width - i*wUnit - spectrumXais.drawStart - 20/root.dpi , 10/root.dpi);
                }else{
                    ctx.text( spectrumXais.format( i*x + min ) , spectrumXais.width - i*wUnit - spectrumXais.drawStart - 3/root.dpi , 10/root.dpi);
                }
            }

            ctx.strokeStyle = '#333333';
            ctx.stroke();
            ctx.fill();
        }
    }

    Rectangle{
        id: spectrumXaisYRect
        anchors.top: spectrumChartRect.top
        anchors.left: spectrumChartRect.right
        width: 30/root.dpi
        height: spectrumChartRect.height
        color: "transparent"
        clip: true

        property int count: chart.sourseType == "sound"?5:5
        Canvas{
            id: spectrumXaisY
            x:0
            y:0
            width: parent.width
            height: spectrumRect.height
            z: 99999

            property string soundType: chart.sourseType
            property real unit_count: chart.sourseType == "sound"?5:5
            // property real len: chart.bandWidth
            // property real unit_y: chart.yunit
            /*
            property real unit_y: spectrumXaisY.len % spectrumXaisYRect.count > 0?
                                      ( spectrumXaisY.len - spectrumXaisY.len % spectrumXaisYRect.count + spectrumXaisYRect.count ) / spectrumXaisY.unit_count
                                    : spectrumXaisY.len / spectrumXaisY.unit_count
            */

            onPaint: {
                var ctx = spectrumXaisY.getContext("2d");

                ctx.clearRect(0,0,spectrumXaisY.width,spectrumXaisY.height);

                ctx.beginPath();
                // ctx.fillStyle="red";
                ctx.font = chart.canvasFont
                ctx.lineWidth=1
                var len = spectrumXaisY.unit_count // 单位y的个数
                var length = chart.bandWidth // 总刻度
                var y = chart.yunit

                var hUnit = spectrumXaisY.height / length * y

                for( var i = 0; i <= len; i++ ){
                    ctx.moveTo( 0 , height - i*hUnit );
                    ctx.lineTo( 3/root.dpi , height - i*hUnit );

                    if( i == 0 ){
                        ctx.text( 0 , 1/root.dpi , height - 3/root.dpi );
                    }else if( i == len ){
                        ctx.text( i*y > length?length:i*y , 1/root.dpi , 10/root.dpi );
                    }else if( i == parseInt(len / 2) + 1 ){
                        ctx.save();
                        context.translate(0, height - i*hUnit);
                        ctx.rotate(90 * Math.PI / 180);
                        ctx.text( "频率(khz)" , 2 , -2);
                        ctx.restore();
                    }else if( i === parseInt(len / 2) ){

                    }else{
                        ctx.text( i*y , 1/root.dpi , height - i*hUnit + 10/root.dpi );
                    }
                }
                ctx.strokeStyle = '#333333';
                ctx.stroke();
                ctx.fill();
            }
        }
    }
}
