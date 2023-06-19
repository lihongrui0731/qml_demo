import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.VirtualKeyboard 2.3

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config
import '../../resources/js/public.js' as JS

Row {
    id: input
    spacing: input.space
    scale: 1/root.dpi

    property string name: ""
    property string value: ""
    property int h: 30 / root.dpi
    property int w: 180 / root.dpi
    property int fontsize: 16 / root.dpi
    property int space: 10 / root.dpi
    property bool center: false
    property bool readOnly: false

    property real inputMethodHints: Qt.ImhNone;

    property int enterKeyAction: EnterKeyAction.None
    readonly property bool enterKeyEnabled: input.enterKeyAction === EnterKeyAction.None || acceptableInput || inputMethodComposing

    signal inputChange( var val );
    signal enterClicked();

    function setValue( v ) {
        input.value = v
        input_value.text = v
    }

    Text {
        height: input.h
        text: qsTr(input.name)
        font.pixelSize: input.fontsize
        verticalAlignment: Text.AlignVCenter
        clip: true
    }

    Controls.TextField {

        id: input_value
        width: input.w
        height: input.h
        font.pixelSize: input.fontsize
        text: qsTr(input.value)
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment:input.center? Text.AlignHCenter:0
        leftPadding: 5
        clip: true
        readOnly:input.readOnly
        selectByMouse: true
        inputMethodHints: input.inputMethodHints
        z:50
        onTextEdited: {
            input.inputChange(input_value.text)
        }
        onTextChanged: {
        }

        activeFocusOnPress:false

        EnterKeyAction.actionId: input.enterKeyAction
        EnterKeyAction.enabled: input.enterKeyEnabled  || acceptableInput || inputMethodComposing

        Keys.onReleased: {
            if( event.key === 16777220 || event.key === 16777221 ){ //回车事件
                input.enterClicked()

                //qt自带键盘 无桌面pad系统不兼容 inputPanel.active = false
            }
        }

        onActiveFocusChanged: {
           // qt自带键盘 无桌面pad系统不兼容  inputPanel.active = false
        }

        Connections{
            id: keyboardConnect
            target: numberKeyboard_
            enabled: false
            function onSureInput( value ){
                input.setValue( value );
                input.enterClicked()
                input.inputChange(value)
                keyboardConnect.enabled = false
            }
        }

        Connections{
            id: fullkeyboardConnect
            target: keyboard_
            enabled: false
            function onSureInput( value ){
                input.setValue( value );
                input.enterClicked()
                input.inputChange(value)
                fullkeyboardConnect.enabled = false
            }
        }

        onPressed: {
            // forceActiveFocus()
            if( input.inputMethodHints == Qt.ImhFormattedNumbersOnly){
                numberKeyboard_.active = true;
                numberKeyboard_.val = input_value.text
                keyboardConnect.enabled = true
            }else{
                keyboard_.active = true;
                keyboard_.val = input_value.text
                fullkeyboardConnect.enabled = true
            }

            return;
            // QT自带键盘逻辑 无桌面pad系统不兼容
            loader.height = root.height

            var pos = JS.getGlobalPosition(input);

            if( pos.y <= root.height / 2  ){
                root.inputX = 50 / root.dpi
                root.inputY = root.height - inputPanel.height

                if( root.height - pos.y - input.height < inputPanel.height ){
                    var dval =  inputPanel.height - (root.height - pos.y )
                    loader.height = loader.height - dval - 100
                }

            }else{
                if( root.height - pos.y - input.height < inputPanel.height ){
                    root.inputX = 50 / root.dpi
                    root.inputY = 0
                }else{
                    root.inputX = 50 / root.dpi
                    root.inputY = root.height - inputPanel.height
                }
            }

            inputPanel.visible = true
            inputPanel_outside.visible = true
            inputPanel.active = true
        }

        background:Rectangle{
            color: UI_color[UI_config[root.usedConfig].colorType].textfield
            border.width: 1
            border.color: input_value.activeFocus ? UI_color[UI_config[root.usedConfig].colorType].text_border_chosen : UI_color[UI_config[root.usedConfig].colorType].text_border
        }
    }
}
