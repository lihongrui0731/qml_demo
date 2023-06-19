import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config

Button{
    id: btn_action
    width:btn_action.w
    height:btn_action.h

    property bool hasPower: root.power
    property bool disabled: false
    property string name: "确认"
    property string bgcolor: btn_action.hasPower?UI_color[UI_config[root.usedConfig].colorType].common_button_bg:UI_color[UI_config[root.usedConfig].colorType].disabled_button_bg;
    property int w: 110/root.dpi
    property int h: 40/root.dpi

    signal actionClicked()

    Label{
        id: btn_action_label
        text:qsTr(name)
        anchors.centerIn: parent
        font.pixelSize: btn_action.h/2
        font.bold: true
        color: UI_color[UI_config[root.usedConfig].colorType].common_button_font
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
        if( !btn_action.hasPower ) return false;
        if( btn_action.disabled ){
            return;
        }
        btn_action.actionClicked(name)
    }
    MouseArea{
        anchors.fill: parent;
        hoverEnabled: true;
        cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
        acceptedButtons: Qt.NoButton
    }

    onPressedChanged: {
        if( btn_action.disabled ){
            return;
        }
        if( btn_action.pressed ){
            btn_back.width = btn_back.width - 2
            btn_back.height = btn_back.height - 2
            btn_action_label.font.pixelSize = btn_action_label.font.pixelSize - 1
        }else{
            btn_back.width = btn_back.width + 2
            btn_back.height = btn_back.height + 2
            btn_action_label.font.pixelSize = btn_action_label.font.pixelSize + 1
        }
    }

}
