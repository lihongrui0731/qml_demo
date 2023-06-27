import QtQuick 2.15

Item {
    id:toou2d_awesomeicon
    width:  16/root.dpi;
    height: 16/root.dpi;

    property color  color;

    /*! \sa TAwesomeType */
    property alias source : content_text.text;

    Text {
        id:content_text
        anchors.fill: parent;
        enabled:          false;
        color:            toou2d_awesomeicon.color;
        font.family:      "fontawesome"
        font.pixelSize:   Math.max(toou2d_awesomeicon.width,toou2d_awesomeicon.height);
        horizontalAlignment: Text.AlignHCenter;
        verticalAlignment:   Text.AlignVCenter; 
    }

}
