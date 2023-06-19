import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12
import FontOutlineLabel 1.0

import "./../element"
import "qrc:/js/color.js" as UI_color
import "qrc:/js/config.js" as UI_config

RoundButton {
    id: btn_menu
    width: btn_menu.w
    height: btn_menu.h

    property string name: ""
    property bool disabled: false
    property string bgcolor: UI_color[UI_config[root.usedConfig].colorType].common_button_bg
    property string iconFont: ""
    property int w: 50/root.dpi
    property int h: 50/root.dpi
    property bool showIcon: UI_config[root.usedConfig].showBtnIcon
    property int fontSize: 14 / root.dpi

    property bool showLineLabel: UI_config[root.usedConfig].showLineLabel
    signal menuButtonClicked
    onNameChanged: {
        btn_menu_label2.setText( btn_menu.name )
    }

    Label {
        id: btn_menu_label
        text: qsTr(name)
        color: UI_color[UI_config[root.usedConfig].colorType].common_button_font
        anchors.centerIn: parent
        font.pixelSize: btn_menu.fontSize
        font.bold: true
        visible: btn_menu.iconFont==""|| !showIcon?true:false
    }

    LineLabel{
        id: btn_menu_label2
        text: qsTr(name)
        fontSize: 14/root.dpi
        fontBold: true
        startPosX: 0
        startPosY: 0
        //startPosY: h / 2 + btn_menu_label2.fontSize / 2
        anchors.centerIn: parent
        fontOutLineWidth: 2
        fontOutLineColor: UI_color[UI_config[root.usedConfig].colorType].common_button_font
        fontColor:  btn_menu.disabled? UI_color[UI_config[root.usedConfig].colorType].disabled_button_bg: bgcolor
        width: btn_menu.w
        height: btn_menu.h
        w: btn_menu.w
        h: btn_menu.h
        z:2
        visible: btn_menu.iconFont!="" && showIcon ?btn_menu.showLineLabel:false
    }

    background: Rectangle {
       id: btn_back
       color: btn_menu.disabled? UI_color[UI_config[root.usedConfig].colorType].disabled_button_bg: bgcolor
       radius: 50
    }

    DropShadow{
        id: btn_shadow
        anchors.fill: btn_back
        radius: 8.0
        samples: 16
        color: UI_color[UI_config[root.usedConfig].colorType].shadow
        source: btn_back
        z: -100
        horizontalOffset: 3
        verticalOffset: 3
    }

    TIcon{
        anchors.centerIn: btn_menu
        width: btn_menu.w * 0.8
        height: btn_menu.h * 0.8
        source: "qrc:/resources/svg/" + iconFont
        visible: (iconFont!="" && showIcon) || (iconFont!="" && name == "" )?true:false
        color: UI_color[UI_config[root.usedConfig].colorType].common_button_font
    }

    MouseArea{
        anchors.fill: parent;
        hoverEnabled: true;
        cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
        acceptedButtons: Qt.NoButton
    }

    onClicked:{
        if( btn_menu.disabled ){
            return;
        }
        btn_menu.menuButtonClicked()
    }

    onPressedChanged: {
        if( btn_menu.disabled ){
            return;
        }
        if( btn_menu.pressed ){
            btn_back.width = btn_back.width - 2
            btn_back.height = btn_back.height - 2
            btn_menu_label.font.pixelSize = btn_menu_label.font.pixelSize - 1
        }else{
            btn_back.width = btn_back.width + 2
            btn_back.height = btn_back.height + 2
            btn_menu_label.font.pixelSize = btn_menu_label.font.pixelSize + 1
        }
    }

}
