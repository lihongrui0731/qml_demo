import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config

Button{
    id: btn_channel
    width: btn_channel.getWidth()
    height:btn_channel.h

    property string name: "按钮"
    property string bgcolor: UI_color[UI_config[root.usedConfig].colorType].choose_botton_bg
    property int w: 70/root.dpi
    property int h: 32/root.dpi

    signal channelButtonClicked(string name)

    Label{
        id: text_
        text:qsTr(name)
        anchors.centerIn: parent
        font.pixelSize: 18/root.dpi
        font.bold: true
        color: "#fff"
    }
    background: Rectangle{
        id: btn_back
        color:bgcolor
        radius: 5/root.dpi
    }

    DropShadow{
        id: btn_shadow
        anchors.fill: btn_back
        radius: 8.0
        samples: 16
        color: UI_color[UI_config[root.usedConfig].colorType].btn_shadow
        source: btn_back
        z: -100
        horizontalOffset: 3
        verticalOffset: 3
    }

    onClicked: {
        channelButtonClicked(name)
    }

    MouseArea{
        anchors.fill: parent;
        hoverEnabled: true;
        cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
        acceptedButtons: Qt.NoButton
    }

    function getWidth(){
        if( text_.width > 60/root.dpi ){
            return text_.width + 20/root.dpi
        }else{
            return btn_channel.w
        }
    }

    onPressedChanged:{
        if( btn_channel.pressed ){
            btn_back.width = btn_back.width - 2
            btn_back.height = btn_back.height - 2
            text_.font.pixelSize = text_.font.pixelSize - 1
        }else{
            btn_back.width = btn_back.width + 2
            btn_back.height = btn_back.height + 2
            text_.font.pixelSize = text_.font.pixelSize + 1
        }
    }

}
