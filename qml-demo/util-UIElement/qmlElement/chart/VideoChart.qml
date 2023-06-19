import QtQuick 2.15
import RhythmVideo 1.0
import GradientBar 1.0

Item {
    id:chart

    property bool isFull: false
    property bool isNightVersion: false
    property int nightMethod: 1
    property string file: ""
    property string channelId: ""
    property bool isUpdate: true
    property bool timeAxis: false

    signal changeTimeCursor( double rate )
    signal getDataSize( int size )
    signal getIntval( int ms );
    signal finish();
    signal frameCountChanged( int count , int time , var timeList );
    signal controlDistanceChanged( int distanceValue );

    signal sendtimeAxis( var timeAxisData )

    function clear(){
        video.resetRhythmVideo();
    }

    function setNightVersion( nightVersion , method ){
        chart.isNightVersion = nightVersion;
        video.nightVersion = nightVersion;
        chart.nightMethod = method;
        video.nightMethod = method;
    }

    function setImageConfig(brightness,contrast,saturation,highLight,shadow,midtone,coefficient){
        video.brightness = brightness
        video.contrast = contrast
        video.saturation = saturation
        video.highLight = highLight
        video.shadow = shadow
        video.midtone = midtone
        video.coefficient = coefficient
    }

    // 多监控点
    function setImageStream( channelId_ , imageData){
        if( channelId_ == chart.channelId && chart.isUpdate ){
            video.addImageStream( imageData );
        }
    }

    function setImageData( channelId_ , imageData ){
         if( channelId_ == chart.channelId && chart.isUpdate ){
             video.addImageData( imageData )
         }
    }
    function setBeamData( channelId_ , beamData ){
        if( channelId_ == chart.channelId && chart.isUpdate ){
             video.addBeamData( beamData )
        }
    }

    Connections {
        target: dm
        function onPictureChanged (d){
            if( root.isSound && !root.isHide ) video.addVideoData( d )
        }
        function onBeamChanged (d){
            if( root.isSound && !root.isHide ) video.addBeamData( d )
        }
        function onCameraChanged(d){
            if( root.isSound && !root.isHide ) video.addImageData( d )
        }
        function onSendFinalVideo( video_ ){
            video.getFinalData( video_ )
        }
        function onSendFinalCamera( camera_ ){
            video.getFinalCameraData( camera_ )
        }
    }

    onFileChanged: {
        // video.openFile(file);
    }

    function setBeamConfig( config ){
        video.setBeamConfig( config.mode , config.dynamicValue , config.peakValue , config.maxValue )
    }

    function changeTimeLine(timeline , currentFrame ){
        video.changeTimeLine(timeline , currentFrame)
    }

    function start(){
        video.start();
    }

    function stop(){
        video.stop();
    }

    function play(){
        video.play();
    }

    function showNext(){
        video.showNext();
    }

    function setData( data ){
        if( data.hasOwnProperty("video") ){
            video.setData( data["video"] );
        }else if( data.hasOwnProperty("camera") ){
            video.setDataWithCamera( data["camera"] );
        }
    }

    Video{
        id:video
        objectName: "videoItem"
        anchors.left: parent.left
        anchors.top: parent.top

        timeAxis: chart.timeAxis

        width: parent.width - chartGradientBar.width - 4/root.dpi
        height: parent.height

        nightVersion: chart.isNightVersion
        nightMethod: chart.nightMethod

       // full: chart.isFull
        onCloudFrameChanged: {
            chartGradientBar.setMinMaxB( video.getMin() , video.getMax() );
            chartGradientBar.setRange( video.getDynamicRange() );
            chartGradientBar.repaint();
        }

        onBeamCountChanged: {
            chart.frameCountChanged( frameCount , frameTime , timeList )
        }

        onDistanceChanged: {
            chart.controlDistanceChanged( distance )
        }

        onFinish: {
            chart.finish();
        }

        onTimeAxisChanged: {
            chart.sendtimeAxis( timeAxis )
        }
    }

    ChartGradientBar{
        id: chartGradientBar
        width: 16/root.dpi
        height: parent.height
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 4/root.dpi
        z:20
        fontSize: 10 / root.dpi
    }

}
