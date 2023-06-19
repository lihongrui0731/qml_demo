import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import "qrc:/js/color.js" as UI_color
import "qrc:/js/config.js" as UI_config

Popup{
    id: numberKeyboard

    modal:false
    focus: true
    closePolicy: Popup.CloseOnPressOutsideParent


    width: 300 / root.dpi
    height:  350 / root.dpi

    scale: root.checkRotation()?-1:1
    transformOrigin: root.checkRotation()?Popup.TopLeft:Popup.Center

    property bool active: false
    property real btnWitdh: 50 / root.dpi
    property real btnHeight: 50 / root.dpi
    property string val: "0"
    signal sureInput(var value);

    background:Rectangle{
        color: UI_color[UI_config[root.usedConfig].colorType].home_left
        border.color: UI_color[UI_config[root.usedConfig].colorType].main_button_bg
        border.width: 2 / root.dpi
        radius: 5 / root.dpi
    }
    onOpened: {
        input_.forceActiveFocus()
    }

    // Grid布局
    TextField{
        id: input_
        x: 2 / root.dpi
        y: 2 / root.dpi
        width: parent.width - 4 / root.dpi
        height: 40 /root.dpi
        text: qsTr(numberKeyboard.val)
        background: Rectangle{
            color: UI_color[UI_config[root.usedConfig].colorType].main
        }
        font.pointSize:  20 / root.dpi
        readOnly: true
        topPadding: 2/ root.dpi
        bottomPadding: 2/ root.dpi

        onTextChanged: {
            numberKeyboard.val = input_.text
        }

        Keys.onReleased: {
            if( !numberKeyboard.active ){
                return;
            }
            input_.forceActiveFocus()

            if( event.key === 48 ){
                // 数字键盘
                if( input_.text == "0" ){
                }else{
                    numberKeyboard.val += (event.key -48)
                }
            }

            if( event.key >= 49 && event.key <= 57 ){
                // 数字键盘
                if( input_.text == "0" ){
                    numberKeyboard.val = (event.key - 48)
                }else{
                    numberKeyboard.val += (event.key -48)
                }
            }
            if( event.key === 16777221 || event.key === 16777220 ){
                // 回车
                numberKeyboard.sureInput( input_.text )
                numberKeyboard.active = false
            }

            if( event.key ===16777216 ){
                // esc
                numberKeyboard.active = false
            }

            if( event.key ===16777219 ){
                // back
                if( numberKeyboard.val.length > 1 ){
                    numberKeyboard.val = input_.text.substring(0,input_.text.length - 1)
                }else{
                    numberKeyboard.val = "0"
                }
            }

            if( event.key === 46 ){
                // .
                if( input_.text.indexOf(".") == -1 ){
                    numberKeyboard.val += "."
                }
            }
        }

    }

    Grid {
        id:grid
        columns: 4
        rows: 4
        spacing:8/root.dpi
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: input_.bottom
        anchors.topMargin: 30 / root.dpi
        // 绘制键盘
        Repeater {
            model: ["7", "8", "9", "⇦",
                    "4", "5", "6", "C",
                    "1", "2", "3", "⇩",
                    "", "0", ".", "√"]
            Button {
                id: btn
                width: numberKeyboard.btnWitdh
                height: numberKeyboard.btnHeight

                onClicked: {
                    switch( modelData ){
                        case "⇦":
                            if( numberKeyboard.val.length > 1 ){
                                numberKeyboard.val = input_.text.substring(0,input_.text.length - 1)
                            }else{
                                numberKeyboard.val = "0"
                            }
                            break;
                        case "⇩":
                            numberKeyboard.active = false
                            break;
                        case "√":
                            numberKeyboard.sureInput( input_.text )
                            numberKeyboard.active = false
                            break;
                        case ".":
                            if( input_.text.indexOf(".") == -1 ){
                                numberKeyboard.val += modelData
                            }
                            break;
                        case "C":
                            numberKeyboard.val = "0"
                            break;
                        default:
                            if( input_.text == "0" ){
                                numberKeyboard.val = modelData
                            }else{
                                numberKeyboard.val += modelData
                            }
                        }
                }

                // 设置键盘文字显示
                Text {
                    text: qsTr(modelData)
                    anchors.centerIn: parent
                    font.pointSize: 25 / root.dpi
                    color:  btn.hovered ?  UI_color[UI_config[root.usedConfig].colorType].font_color : UI_color[UI_config[root.usedConfig].colorType].common_button_font
                }
                // 设置键盘背景以及辉光效果
                background: Rectangle {
                    id: bckRect
                    anchors.centerIn: parent
                    color: btn.hovered ?  UI_color[UI_config[root.usedConfig].colorType].main : UI_color[UI_config[root.usedConfig].colorType].main_button_bg
                    radius: 4
                    layer.enabled: true
                    layer.effect: Glow{
                        id: glow
                        anchors.fill: bckRect
                        radius: 10
                        samples: 17
                        color: UI_color[UI_config[root.usedConfig].colorType].common_button_bg
                        source: bckRect
                        SequentialAnimation{
                            running: true
                            loops: Animation.Infinite
                            NumberAnimation {
                                target: glow
                                property: "spread"
                                to: 0
                                duration: 1000
                            }
                            NumberAnimation {
                                target: glow
                                property: "spread"
                                to: 0.5
                                duration: 1000
                            }
                        }
                    }
                }
            }
        }
    }
}
