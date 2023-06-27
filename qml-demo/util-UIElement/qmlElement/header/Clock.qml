import QtQuick 2.15
import QtQuick.Controls 2.15

Item{
    id: clock
    Label{
        id: clock_str
        text: qsTr("")
        font.pixelSize: 16/root.dpi
        anchors.right: clock.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 10/root.dpi
    }

    Component.onCompleted: {
        clock_timer.start()
    }

    function getTime(){
        clock_str.text = new Date().toLocaleString(Qt.locale(),"yyyy-MM-dd hh:mm");
    }

    Timer{
        id: clock_timer
        interval: 60000
        running: false
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            clock.getTime()
        }
    }

}
