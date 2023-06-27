import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15
import QtQuick.Templates 2.15 as T
import "qrc:/js/color.js" as UI_color
import 'qrc:/js/config.js' as UI_config

T.ComboBox {
    id: control;

    padding: 0

    implicitWidth: implicitBackgroundWidth/root.dpi;
    implicitHeight: implicitBackgroundHeight/root.dpi;
    leftPadding: padding + (!control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing);
    rightPadding: padding + (control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing);

    // 可在此定义多为改变的特殊属性，在调用时直接指定此属性即可
    font.pixelSize: 12/root.dpi;

    property color fontColor: UI_color[UI_config[root.usedConfig].colorType].font_color; // 字体颜色
    property color backgroundColor: "transparent"; // 背景色
    property color borderColor: UI_color[UI_config[root.usedConfig].colorType].border

    // 弹出框行委托
    delegate: ItemDelegate {
        width: parent.width;
        height: 44/root.dpi;
        // 行字体样式

        contentItem: Text {
            text: textRole=="text"?modelData.text:modelData;
            font: control.font;
            color: control.fontColor
            elide: Text.ElideRight;
            width: parent.width
            horizontalAlignment: Text.AlignLeft;
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            anchors.leftMargin: 10
            height:  44/root.dpi
            renderType: Text.NativeRendering;
            z:99999
        }

        palette.text: control.palette.text;
        palette.highlightedText: control.palette.highlightedText;
        font.weight: control.currentIndex === index ? Font.DemiBold : Font.Normal;
        highlighted: control.highlightedIndex === index;
        hoverEnabled: control.hoverEnabled;
    }

    // 右侧下拉箭头
    indicator: Canvas {
        id: canvas;
        x: control.width - width - control.rightPadding;
        y: control.topPadding + (control.availableHeight - height) / 2;
        width: 10/root.dpi;
        height: 6/root.dpi;
        contextType: "2d";

        Connections {
            target: control;
            function onPressedChanged(){
                canvas.requestPaint();
            }
        }
        onPaint: {
            context.reset();
            context.moveTo(0, 0);

            context.lineWidth = 2/root.dpi;
            context.lineTo(width / 2, height*0.8);
            context.lineTo(width, 0);
            context.strokeStyle = control.pressed ? "#EEEFF7" : UI_color[UI_config[root.usedConfig].colorType].font_color;
            context.stroke();
        }
    }

    // ComboBox的文字位置样式
    contentItem: T.TextField {
        leftPadding: !control.mirrored ? 12/root.dpi : control.editable && activeFocus ? 3/root.dpi : 1/root.dpi;
        rightPadding: control.mirrored ? 12/root.dpi : control.editable && activeFocus ? 3/root.dpi : 1/root.dpi;
        topPadding: control.padding;
        bottomPadding: control.padding;

        text: control.editable ? control.editText : control.displayText;

        enabled: control.editable;
        autoScroll: control.editable;
        readOnly: control.down;
        inputMethodHints: control.inputMethodHints;
        validator: control.validator;

        font: control.font;
        color: control.fontColor;
        //color: control.editable ? control.palette.text : control.palette.buttonText
        selectionColor: control.palette.highlight;
        selectedTextColor: control.palette.highlightedText;
        verticalAlignment: Text.AlignVCenter;
        renderType: Text.NativeRendering;

        background: Rectangle {
            visible: control.enabled && control.editable && !control.flat;
            border.width: parent && parent.activeFocus ? 2/root.dpi : 1/root.dpi;
            border.color: control.borderColor
            color: control.palette.base;
        }
    }

    // ComboBox 的背景样式
    background: Rectangle {
        implicitWidth: 120/root.dpi;
        implicitHeight: 30/root.dpi;

        radius: 3;
        color: control.backgroundColor;
        border.color: control.borderColor;
        border.width: !control.editable && control.visualFocus ? 2/root.dpi : 1/root.dpi;
        visible: !control.flat || control.down;
    }

    // 弹出窗口样式
    popup: T.Popup {
        y: control.height;
        width: control.width;
        //height: Math.min(contentItem.implicitHeight, control.Window.height - topMargin - bottomMargin);
        height: contentItem.implicitHeight;
        topMargin: 3/root.dpi;
        bottomMargin: 3/root.dpi;

        scale: root.checkRotation()? -1 :1
        transformOrigin: root.checkRotation()?Popup.TopLeft:Popup.Center

        contentItem: ListView {
            // 防止显示过界
            clip: true;
            //禁止滑动
            // interactive: false;
            //禁用橡皮筋效果
            boundsBehavior: Flickable.StopAtBounds;

            implicitHeight: contentHeight;
            model: control.delegateModel;
            currentIndex: control.highlightedIndex;
            highlightMoveDuration: 0;



            Rectangle {
                z: 10;
                width: parent.width;
                height: parent.height;
                color: "transparent";
                border.color: control.palette.mid;
            }

            T.ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            color: control.pressed ? "#EEEFF7" : control.palette.window;
            border.width: 1/root.dpi;
            border.color: control.backgroundColor;
            radius: 3;
        }
    }
}
