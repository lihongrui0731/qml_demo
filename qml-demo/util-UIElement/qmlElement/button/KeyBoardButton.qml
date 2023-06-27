import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0

import "qrc:/js/color.js" as UI_color
import "qrc:/js/config.js" as UI_config

Button {
    id: btn
    property string model: ""

    // 设置键盘文字显示
    Text {
        text: qsTr(model)
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
