import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import "qrc:/js/color.js" as UI_color
import "qrc:/js/config.js" as UI_config
import "../button"

Popup{
    id: keyboard

    modal:false
    focus: true
    closePolicy: Popup.CloseOnPressOutsideParent

    scale: root.checkRotation()? -1 :1
    transformOrigin: root.checkRotation()?Popup.TopLeft:Popup.Center

    width: 800 / root.dpi
    height:  350 / root.dpi

    property bool active: false
    property real btnWitdh: 50 / root.dpi
    property real btnHeight: 50 / root.dpi
    property string val: ""
    property bool upper: false
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
        text: qsTr(keyboard.val)
        background: Rectangle{
            color: UI_color[UI_config[root.usedConfig].colorType].main
        }
        font.pointSize:  20 / root.dpi
        topPadding: 2 / root.dpi
        bottomPadding: 2 / root.dpi
        focusReason:Qt.OtherFocusReason

        onTextChanged: {
            keyboard.val = input_.text
        }

        Keys.onReleased: {
            if( event.key === 16777221 || event.key === 16777220 ){
                // 回车
                keyboard.sureInput( input_.text )
                keyboard.active = false
            }
            if( event.key ===16777216 ){
                // esc
                keyboard.active = false
            }
        }

    }

    Column{
        spacing:8/root.dpi
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: input_.bottom
        anchors.topMargin: 30 / root.dpi

        Row {
            id: first
            spacing:8/root.dpi
            // 绘制键盘
            property var bigModel: ["Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "⇦"]
            property var smallModel: ["q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "⇦"]
            property var numberModel: ["1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "⇦"]
            property var model: first.smallModel
            Repeater {
                model: first.model
                KeyBoardButton{
                    width: keyboard.btnWitdh
                    height: keyboard.btnHeight
                    model: modelData

                    onClicked: {
                        if ( modelData == "⇦" ){
                            if( keyboard.val.length > 1 ){
                                keyboard.val = input_.text.substring(0,input_.text.length - 1)
                            }else{
                                keyboard.val = ""
                            }
                        }else{
                            keyboard.val += modelData
                        }
                        input_.forceActiveFocus();
                    }
                }
            }
        }

        Row {
            id: second
            spacing:8/root.dpi
            // 绘制键盘
            property var bigModel: ["A", "S", "D", "F", "G", "H", "J", "K", "L"]
            property var smallModel: ["a", "s", "d", "f", "g", "h", "j", "k", "l"]
            property var numberModel: ["!", "@", "#", "$" , "%", "^", "&", "*", "|" ]
            property var model: second.smallModel
            Repeater {
                model: second.model
                KeyBoardButton{
                    width: keyboard.btnWitdh
                    height: keyboard.btnHeight
                    model: modelData
                    onClicked: {
                        keyboard.val += modelData
                        input_.forceActiveFocus();
                    }
                }
            }

            KeyBoardButton{
                width: keyboard.btnWitdh * 2 + second.spacing
                height: keyboard.btnHeight
                model: "√"
                onClicked: {
                    keyboard.sureInput( input_.text )
                    keyboard.active = false
                }
            }
        }

        Row {
            id: third
            spacing:8/root.dpi
            // 绘制键盘
            property var bigModel: [ "⇧", "Z", "X", "C", "V", "B", "N", "M", "，", "。" , "CE"]
            property var smallModel: ["⇧" , "z", "x", "c", "v", "b", "n", "m", ",", "." , "CE"]
            property var numberModel: ["-" , "=", "_", "+", "<", ">", ":", ";" , "(", ")" , "CE"]
            property var model: third.smallModel
            Repeater {
                model: third.model
                KeyBoardButton{
                    width: keyboard.btnWitdh
                    height: keyboard.btnHeight
                    model: modelData
                    onClicked: {
                        if( modelData == "⇧" ){
                            if( !keyboard.upper ){
                                keyboard.upper = true;
                                first.model = first.bigModel
                                second.model = second.bigModel
                                third.model = third.bigModel
                            }else{
                                keyboard.upper = false;
                                first.model = first.smallModel
                                second.model = second.smallModel
                                third.model = third.smallModel
                            }
                        }else if( modelData == "CE" ){
                            keyboard.val = "";
                        }else{
                            keyboard.val += modelData
                        }
                        input_.forceActiveFocus();
                    }
                }
            }
        }

        Row {
            id: fourth
            spacing:8/root.dpi

            KeyBoardButton{
                id: symbolBtn
                width: keyboard.btnWitdh * 2
                height: keyboard.btnHeight
                model: "&123"
                visible: true;
                onClicked: {
                    first.model = first.numberModel
                    second.model = second.numberModel
                    third.model = third.numberModel

                    symbolBtn.visible = false
                    enBtn.visible = true

                    input_.forceActiveFocus();
                }
            }

            KeyBoardButton{
                id: enBtn
                width: keyboard.btnWitdh * 2
                height: keyboard.btnHeight
                model: "ABC"
                visible: false;
                onClicked: {
                    keyboard.upper = false;
                    first.model = first.smallModel
                    second.model = second.smallModel
                    third.model = third.smallModel

                    symbolBtn.visible = true
                    enBtn.visible = false

                    input_.forceActiveFocus();
                }
            }


            KeyBoardButton{
                width: keyboard.btnWitdh
                height: keyboard.btnHeight
                model: "EN"
                onClicked: {
                    input_.forceActiveFocus();
                }
            }

            KeyBoardButton{
                width: keyboard.btnWitdh * 6 - fourth.spacing
                height: keyboard.btnHeight
                model: "空格"
                onClicked: {
                    keyboard.val += " "
                    input_.forceActiveFocus();
                }
            }

            KeyBoardButton{
                width: keyboard.btnWitdh
                height: keyboard.btnHeight
                model: "?"
                onClicked: {
                    keyboard.val += "?"
                    input_.forceActiveFocus();
                }
            }

            KeyBoardButton{
                width: keyboard.btnWitdh
                height: keyboard.btnHeight
                model: "/"
                onClicked: {
                    keyboard.val += "/"
                    input_.forceActiveFocus();
                }
            }

            KeyBoardButton{
                width: keyboard.btnWitdh
                height: keyboard.btnHeight
                model: "⇩"
                onClicked: {
                    keyboard.active = false
                }
            }

        }
    }

}
