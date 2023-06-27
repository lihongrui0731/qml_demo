import QtQuick 2.15

Item {
    id:toou2d_svgicon
    width:  16/root.dpi;
    height: 16/root.dpi;

    property bool   asynchronous : true;
    property bool   smooth: true;
    property color  color;
    property string source;
    property alias status: image.status;

    Image {
        id:image
        asynchronous: toou2d_svgicon.asynchronous;
        anchors.fill: toou2d_svgicon;
        source:  toou2d_svgicon.source;
        smooth: toou2d_svgicon.smooth
        visible: false;
        enabled: false;
    }

    ShaderEffect {
        id: shaderItem
        property variant source: image
        property color   color: toou2d_svgicon.color
        enabled: false;
        fragmentShader: "qrc:/resources/font/svg.cso"
        anchors.fill: parent;
        visible: toou2d_svgicon.status === Image.Ready;
    }
}
