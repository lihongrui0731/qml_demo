import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15
import QtQuick.Templates 2.15 as T

import "qrc:/js/color.js" as UI_color
import 'qrc:/js/config.js' as UI_config

T.CheckBox {
    id: checkboxBtn;

    // 自定义控件属性
    property int checkBoxSize: 30/root.dpi;  // CheckBox大小 默认正方形
    // 复选框字体属性
    property int fontSize: 16/root.dpi; // 复选框 文字大小
    // property string fontFamily: "Microsoft YaHei";  // 文字字体
    property color fontColor: "#333333"; // 字体颜色
    //  此处还可定义复选框选中颜色等属性

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding); // 适应当前control宽
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding); // 适应当前control高
    padding: 6/root.dpi;
    spacing: 3/root.dpi; // 间距

    // CheckBox 外形属性定义
    // 此处也可直接用jpg png svg等格式Image
    indicator: Rectangle {
        id:indicator_
        width: checkBoxSize;
        height: checkBoxSize; // 长宽
        anchors.verticalCenter: parent.verticalCenter; // 位置
        color: checkboxBtn.enabled ? "#ffffff" : "transparent";  // 颜色，使能与不使能
        border.color: checkboxBtn.down ? UI_color[UI_config[root.usedConfig].colorType].common_button_bg : "#cccccc";// 选中边框颜色
        border.width: 1
        radius: 2/root.dpi;

        Rectangle {
            id: indicator_dot
            width: checkBoxSize - 10/root.dpi
            height: checkBoxSize - 10 /root.dpi
            x: ( indicator_.width - indicator_dot.width ) /2
            y: ( indicator_.height - indicator_dot.height ) /2
            color: checkboxBtn.checked ? UI_color[UI_config[root.usedConfig].colorType].common_button_bg : "transparent"
        }
    }

    // CheckBox 名称属性定义
    contentItem: Text {
        color: fontColor;   // 字体颜色
        text: checkboxBtn.text; //
        font.pixelSize: fontSize;  // 大小
        leftPadding: checkboxBtn.indicator.width + checkboxBtn.spacing; // 左间距
        renderType: Text.NativeRendering;  // 防止 Controls 2 字体模糊
        horizontalAlignment: Text.AlignHCenter;
        verticalAlignment: Text.AlignHCenter; // 文字位置
    }
}
