import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle{
    id: loading
    anchors.fill: parent
    color:bgcolor
    z:9998

    property int time:  1000
    property color bgcolor:  Qt.rgba(200/255,200/255,200/255,0.8)
    signal timeout()

    function delay( times ){
        loading.time = times;
        timer.start();
    }

    /*
    AnimatedImage {
        anchors.centerIn: parent
        source: "./../icon/loading.gif"
        width: 100/root.dpi
        fillMode: Image.PreserveAspectFit
        z: 9999
        asynchronous:true
    }
    */

    Text{
        text: "加载中......"
        font.pixelSize: 30 / root.dpi
        anchors.centerIn: parent
    }

    MouseArea{
        anchors.fill: parent
        cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
    }

    Timer{
        id: timer
        repeat: false
        running: false
        interval: loading.time
        onTriggered: {
           loading.timeout()
        }
    }

}
