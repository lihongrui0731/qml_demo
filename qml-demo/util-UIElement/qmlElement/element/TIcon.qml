import QtQuick 2.15

Item {
    id:toou2d_icon

    property string source;
    property color  color;

    Loader{
        anchors.fill: parent;
        sourceComponent: {
            if(toou2d_icon.source.toLowerCase().indexOf(".svg") !== -1){
                return svgComponent;
            }
            return awesomeiconComponent;
        }
    }

    Component{
        id:awesomeiconComponent
        TAwesomeIcon{
            enabled: false;
            source: toou2d_icon.source;
            color:  toou2d_icon.color;
            anchors.fill: parent;
        }
    }

    Component{
        id:svgComponent
        TSVGIcon{
            enabled: false;
            source: toou2d_icon.source;
            color:  toou2d_icon.color;
            anchors.fill: parent;
        }
    }
}
