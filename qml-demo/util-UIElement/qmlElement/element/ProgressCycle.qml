import QtQuick 2.15
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

// draws two arcs (portion of a circle)
// fills the circle with a lighter secondary color
// when pressed

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config

Canvas {
    id: canvas
    width: 60
    height: 60
    antialiasing: true

    property color fontColor: "#333333"
    property color primaryColor: UI_color[UI_config[root.usedConfig].colorType].choose_button_bg_off
    property color secondaryColor: UI_color[UI_config[root.usedConfig].colorType].choose_button_bg_on

    property real centerWidth: width / 2
    property real centerHeight: height / 2
    //property real radius: Math.min(canvas.width, canvas.height) / 2
    property real radius: Math.min(canvas.width-10, canvas.height-10) / 2

    property real minimumValue: 0
    property real maximumValue: 100
    property real currentValue: 0

    property bool auto: false

    // this is the angle that splits the circle in two arcs
    // first arc is drawn from 0 radians to angle radians
    // second arc is angle radians to 2*PI radians
    property real angle: (currentValue - minimumValue) / (maximumValue - minimumValue) * 2 * Math.PI

    // we want both circle to start / end at 12 o'clock
    // without this offset we would start / end at 9 o'clock
    property real angleOffset: -Math.PI / 2

    signal clicked()
    onPrimaryColorChanged: requestPaint()
    onSecondaryColorChanged: requestPaint()
    onMinimumValueChanged: requestPaint()
    onMaximumValueChanged: requestPaint()
    onCurrentValueChanged: requestPaint()
    onPaint: {
        var ctx = getContext("2d");
        ctx.save();

        ctx.clearRect(0, 0, canvas.width, canvas.height);

        // fills the mouse area when pressed
        // the fill color is a lighter version of the
        // secondary color

        if (mouseArea.pressed) {
            ctx.beginPath();
            ctx.lineWidth = 10;
            ctx.fillStyle = Qt.lighter(canvas.secondaryColor, 1.25);
            ctx.arc(canvas.centerWidth,
                    canvas.centerHeight,
                    canvas.radius,
                    0,
                    2*Math.PI);
            ctx.fill();

            timer.running = true;
        }

        // First, thinner arc
        // From angle to 2*PI

        ctx.beginPath();
        ctx.lineWidth = 10;
        ctx.strokeStyle = primaryColor;
        ctx.arc(canvas.centerWidth,
                canvas.centerHeight,
                canvas.radius,
                angleOffset + canvas.angle,
                angleOffset + 2*Math.PI);
        ctx.stroke();


        // Second, thicker arc
        // From 0 to angle

        ctx.beginPath();
        ctx.lineWidth = 10;
        ctx.strokeStyle = canvas.secondaryColor;
        ctx.arc(canvas.centerWidth,
                canvas.centerHeight,
                canvas.radius,
                canvas.angleOffset,
                canvas.angleOffset + canvas.angle);
        ctx.stroke();

        ctx.restore();
    }

    Text {
        id: txt_progress
        anchors.centerIn: parent

        font.pixelSize: canvas.width / 3
        text: qsTr(String(canvas.currentValue))
        color: canvas.fontColor
    }

    MouseArea {
        id: mouseArea
        visible: canvas.auto
        anchors.fill: parent
        onClicked: canvas.clicked()
        onPressedChanged: canvas.requestPaint()

    }

    Timer{
        id: timer
        interval: 300;
        running: false;
        repeat: true;
        onTriggered: {
            if(currentValue === 100) {
                currentValue = 0;
                txt_progress.text = "0"
            }
            currentValue += 1;
            txt_progress.text = currentValue.toString()+"%";
        }

    }
}
