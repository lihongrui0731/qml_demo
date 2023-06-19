import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15
import QtQuick.Templates 2.15 as T

import "qrc:/js/color.js" as UI_color
import 'qrc:/js/config.js' as UI_config
//自定义单选按钮
T.RadioButton {
    id: radioBtn
    text: qsTr("")

    property int checkBoxSize: 30/root.dpi;  // CheckBox大小 默认正方形
    // 复选框字体属性
    property int fontSize: 16/root.dpi; // 复选框 文字大小
    // property string fontFamily: "Microsoft YaHei";  // 文字字体
    property color fontColor: "#333333"; // 字体颜色

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding); // 适应当前control宽
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding); // 适应当前control高
    padding: 6/root.dpi;
    spacing: 3/root.dpi; // 间距

    indicator: Rectangle {
        id: indicator_
        implicitWidth: checkBoxSize
        implicitHeight: checkBoxSize
        x: radioBtn.leftPadding
        y: parent.height / 2 - height / 2
        radius: 28
        border.color: radioBtn.down ? UI_color[UI_config[root.usedConfig].colorType].common_button_bg : "#cccccc"

        Rectangle {
            id: indicator_dot
            width: checkBoxSize - 10/root.dpi
            height: checkBoxSize - 10 /root.dpi
            x: ( indicator_.implicitWidth - indicator_dot.width ) /2
            y: ( indicator_.implicitHeight - indicator_dot.height ) /2
            radius: 17
            color: radioBtn.checked ? UI_color[UI_config[root.usedConfig].colorType].common_button_bg : "transparent"
        }
    }

    contentItem: Text {
        text: radioBtn.text
        font: radioBtn.font
        opacity: enabled ? 1.0 : 0.3
        color: fontColor
        verticalAlignment: Text.AlignVCenter
        leftPadding: radioBtn.indicator.width + radioBtn.spacing
    }
}
