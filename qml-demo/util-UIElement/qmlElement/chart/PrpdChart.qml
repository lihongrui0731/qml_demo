import QtQuick 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15
import ScatterChart 1.0

import 'qrc:/js/color.js' as UI_color
import '../../resources/js/public.js' as Js

Item {
    id: scatterChart
    property bool isFull: false
    property var dataArr:({})
    property bool disabled: false
    property string backgroundColor: root.cardColor

    function changeTimeLine( timeline ){
        if( scatterChart.disabled ) return;
        schart.changeTimeLine( timeline );
    }

    function start(){
        if( scatterChart.disabled ) return;
        schart.reset();
    }

    function setData(data_){
        schart.setData( data_ );
    }

    function addData(data_){
        schart.addData(data_);
    }

    Connections {
        target: dm
        function onPrpdChanged (s){
            if( scatterChart.disabled ) return;

            if( root.isHide || !root.isSound ) return;
            schart.addData(s);
        }

        function onSendFinalPRPD( prpd_ ){
            if( scatterChart.disabled ) return;
            schart.setData( prpd_ );
        }

        function onSendSoundData(data){
           if( scatterChart.disabled ) return;
           if( data.hasOwnProperty( "prpd" ) ){
                schart.setData( data["prpd"] )
            }
        }
        function onReprocessPrpdFinish( prpd ){
            if( scatterChart.disabled ) return;
            schart.setData( prpd );
        }

    }

    Rectangle{
        anchors.fill: parent
        color: "#CCCCCC"
        z:3
        visible: scatterChart.disabled

        Text{
            text: qsTr("未开通")
            anchors.centerIn: parent
            font.pixelSize: 20 / root.dpi
        }
    }

    ScatterChart{
        id: schart
        anchors.fill: parent
        fontSize: 10 / root.dpi
        dpi: root.dpi
        useOpengl: root.useOpengl
        textColor: "#aaaaaa"
        bgColor: scatterChart.backgroundColor
        Component.onCompleted: {
            schart.rePaint()
        }
    }

}
