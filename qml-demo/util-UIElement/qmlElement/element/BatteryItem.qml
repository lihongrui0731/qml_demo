import QtQuick 2.15
import QtQuick.Controls 2.15
import Battery 1.0

Item {
    id: bat
    property int last_battery_power: 0
    property int battery_power: 0
    //显示剩余电量百分比
    property int power_percent: -1
    //显示电压
    property int battery_voltage: 0

    Component.onCompleted: {

    }

    function setBattery( capacity_ , voltage_ ){
        if( Math.round( capacity_ ) == bat.power_percent ){
            return;
        }
        bat.power_percent = Math.round( capacity_ );
        bat.battery_voltage = Math.round(voltage_);

        if( bat.power_percent > 75 ){
            bat.battery_power = 3
            img.source = "qrc:/resources/icon/battery/full.ico"
        }else if( bat.power_percent > 50 ){
            bat.battery_power = 2
            img.source = "qrc:/resources/icon/battery/large.ico"
        }else if( bat.power_percent > 25 ) {
            bat.battery_power = 1
            img.source = "qrc:/resources/icon/battery/half.ico"
        }else if( bat.power_percent > 5 ) {
            bat.battery_power = 0
            img.source = "qrc:/resources/icon/battery/litte.ico"
        }else{
            bat.battery_power = -1
            img.source = "qrc:/resources/icon/battery/none.ico"
        }

    }

    //方法1：切图片
    Rectangle{
        width: parent.width/2
        height: parent.height/2
        color: "transparent"
        Image {
            id: img
            anchors.centerIn: parent
            width: bat.width
            height: bat.height
            source: "qrc:/resources/icon/battery/none.ico"
            asynchronous: true
            Text{
                text: bat.power_percent.toString() + "%"
                font.pixelSize: 10/root.dpi
                //anchors.top: img.top
                //anchors.right: img.right
                anchors.centerIn: parent
                visible: bat.power_percent> 0 && bat.power_percent <= 100 ? true:false
            }
        }
    }
}
