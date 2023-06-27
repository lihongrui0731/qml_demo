import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: bat
    property int last_battery_power: 0
    property int battery_power: 0
    //显示剩余电量百分比
    property int power_percent: 0
    //显示电压
    property int battery_voltage: 0
    property bool isCharge:false
    property int warnCount: 0


    Component.onCompleted: {
        bat.getBattery()
    }

    function setBattery( bat_ ){
        if( bat_.length <= 0 ){
            return;
        }
        if( bat.power_percent !== bat_[0] ){
            bat.power_percent = bat_[0];
        }
        if( bat_[1] === 128 ){
            bat.battery_power = 3
            if( !bat.isCharge ){
                bat.isCharge = true;
                img.source = "qrc:/resources/icon/battery/charging.ico"
            }
        }else if( bat_[1] === 8 ){
            bat.battery_power = bat.battery_power + 1
            if(bat.battery_power > 3 )
            {
                bat.battery_power = 0
                img.source = "qrc:/resources/icon/battery/none.ico"
            }
        }else{
            if( bat.power_percent  > 75 ){
                if( bat.battery_power != 3 || bat.isCharge ){
                    bat.battery_power = 3
                    img.source = "qrc:/resources/icon/battery/full.ico"
                    bat.isCharge = false
                }
            }else if( bat.power_percent  > 50 ){
                if( bat.battery_power != 2 || bat.isCharge ){
                    bat.battery_power = 2
                    img.source = "qrc:/resources/icon/battery/large.ico"
                    bat.isCharge = false
                }
            }else if( bat.power_percent  > 25 ){
                if( bat.battery_power != 1 || bat.isCharge ){
                    bat.battery_power = 1
                    img.source = "qrc:/resources/icon/battery/half.ico"
                    bat.isCharge = false
                }
            }else if( bat.power_percent  > 5 ) {
                if( bat.battery_power != 0 || bat.isCharge ){
                    bat.battery_power = 0
                    img.source = "qrc:/resources/icon/battery/little.ico"
                    bat.isCharge = false
                }
            }else{
                if( bat.battery_power != -1 || bat.isCharge ){
                    bat.battery_power = -1
                    img.source = "qrc:/resources/icon/battery/none.ico"
                    bat.isCharge = false
                }
            }
            if( bat.power_percent  < 10 ){
                bat.warnCount++
                if( bat.warnCount == 60 ){
                    messageDialog.show( "电量低，请及时保存文件，\n避免数据丢失" );
                    bat.warnCount = 0;
                }
            }
        }
    }

    function getBattery(){
        var bat_ = battery_.getBattery()

        bat.setBattery(bat_);
    }

    Timer{
        id: timer
        interval: 1000
        running: true
        repeat: true
        triggeredOnStart: true
    }

    Connections{
        target: timer
        function onTriggered() {
            bat.last_battery_power = bat.battery_power
            bat.getBattery()
        }
    }

    Connections{
       target: cm
       function onBatteryInfoChanged( type , value ){
           timer.stop();
           var bat_ = [];
           if( type === "charging" ){
               if( value === 1 ){
                   bat_[0] = bat.power_percent;
                   bat_[1] = 128;
               }else{
                   bat_[0] = bat.power_percent;
                   bat_[1] = 0;
               }
           }else if( type === "level" ){
               bat_[0] = value;
               bat_[1] = bat.isCharge?128:0;
           }
           bat.setBattery(bat_);
           bat.setBattery( bat_ );
           battery_.saveBattery( bat_[0] , bat_[1] );
       }
   }

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
                visible:( bat.power_percent> 0 && bat.power_percent <= 100 ) ? true:false
            }
        }
    }
}
