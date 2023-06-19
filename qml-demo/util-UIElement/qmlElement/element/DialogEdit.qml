import QtQuick 2.15
import QtQuick.Controls 2.15
Popup {
    id: dialEdit
    width: 400 / root.dpi
    height: 200 / root.dpi
    x: parent.width/2 - dialEdit.width/2
    y: parent.height/2 - dialEdit.height
    z:9999
    modal: false
    focus: true

    scale: root.checkRotation()? -1 :1
    transformOrigin: root.checkRotation()?Popup.TopLeft:Popup.Center

    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.CloseOnEscape

    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect
    property string title: "?";
    signal doSure;
    property string value: new Date().toLocaleString(Qt.locale(),"yyyy-MM-dd_HH-mm-ss")

    Rectangle {
        id: rect
        anchors.fill: parent
        color: "#eeeeee"
        border.width: 1
        opacity: 1
        radius: 8
        Rectangle{
            id: titleBorder
            width: parent.width-4
            height: 2
            anchors.top: parent.top
            anchors.topMargin: 40 / root.dpi
            anchors.left: parent.left
            anchors.leftMargin: 2
            radius: 8
        }
        //设置标题栏区域为拖拽区域
        Text {
            width: parent.width
            height: 40 / root.dpi
            anchors.top: parent.top
            text: qsTr(dialEdit.title)
            font.pixelSize: 20 / root.dpi
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            MouseArea {
                property point clickPoint: "0,0"
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                cursorShape: root.showMouse?Qt.PointingHandCursor:Qt.BlankCursor
                onPressed: {
                    clickPoint  = Qt.point(mouse.x, mouse.y)
                }
                onPositionChanged: {
                    var offset = Qt.point(mouse.x - clickPoint.x, mouse.y - clickPoint.y)
                    setDlgPoint(offset.x, offset.y)
                }
            }
        }
        Input {
            id: input
            value: dialEdit.value
            // anchors.left: parent.left
            anchors.top: titleBorder.bottom
            //anchors.leftMargin: 50 / root.dpi
            anchors.topMargin: 20 / root.dpi
            anchors.horizontalCenter: parent.horizontalCenter

            w: 300
            h: 50
            onInputChange: {
                dialEdit.value = val
            }
        }
        Button {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 50 / root.dpi
            anchors.bottomMargin: 30 / root.dpi
            text: qsTr("确定")
            font.pixelSize: 20 / root.dpi
            width: parent.width / 3
            height: 50 / root.dpi
            onClicked: {
                dialEdit.doSure()
                dialEdit.close()
            }
        }
        Button {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 50 / root.dpi
            anchors.bottomMargin: 30 / root.dpi
            width: parent.width / 3
            height: 50 / root.dpi
            text: qsTr("取消")
            font.pixelSize: 20 / root.dpi
            onClicked: {
                dialEdit.close()
            }
        }
    }
    function setDlgPoint(dlgX ,dlgY)
    {
        //设置窗口拖拽不能超过父窗口
        if(dialEdit.x + dlgX < 0)
        {
            dialEdit.x = 0
        }
        else if(dialEdit.x + dlgX > dialEdit.parent.width - dialEdit.width)
        {
            dialEdit.x = dialEdit.parent.width - dialEdit.width
        }
        else
        {
            dialEdit.x = dialEdit.x + dlgX
        }
        if(dialEdit.y + dlgY < 0)
        {
            dialEdit.y = 0
        }
        else if(dialEdit.y + dlgY > dialEdit.parent.height - dialEdit.height)
        {
            dialEdit.y = dialEdit.parent.height - dialEdit.height
        }
        else
        {
            dialEdit.y = dialEdit.y + dlgY
        }
    }
}
