import QtQuick 2.15
import QtCharts 2.3
import QtQuick.Controls 2.15
import QtDataVisualization 1.2
import '.'

Item {
    id: surfaceView
    anchors.fill: parent

    // 模拟数据
    HeatmapData {
        id: surfaceData
    }

    ColorGradient {
        id: surfaceGradient
        ColorGradientStop { position: 0.0; color: "black" }
        ColorGradientStop { position: 0.2; color: "red" }
        ColorGradientStop { position: 0.5; color: "blue" }
        ColorGradientStop { position: 0.8; color: "yellow" }
        ColorGradientStop { position: 1.0; color: "white" }
    }

    ValueAxis3D {
        id: xAxis
        segmentCount: 8
        labelFormat: "%i"
        //labelFormat: ""
        title: "时间"
        titleVisible: false
        titleFixed: false
    }

    ValueAxis3D {
        id: yAxis
        segmentCount: 8
        labelFormat: "%i"
        // labelFormat: ""
        title: "频率"
        titleVisible: false
        labelAutoRotation: 0
        titleFixed: false
    }

    ValueAxis3D {
        id: zAxis
        segmentCount: 8
        labelFormat: "%i"
        //labelFormat: ""
        title: "强度"
        titleVisible: false
        titleFixed: false
    }

    Surface3D {
        id: surfaceGraph
        height: surfaceView.height - 10/root.dpi
        width: surfaceView.width - 60/root.dpi
        anchors.left: parent.left
        anchors.top: parent.top
        margin: 0

        shadowQuality: AbstractGraph3D.ShadowQualityNone
        selectionMode: AbstractGraph3D.SelectionSlice | AbstractGraph3D.SelectionItemAndColumn
        axisX: xAxis
        axisY: yAxis
        axisZ: zAxis

        // Remove the perspective and view the graph from top down to achieve 2D effect
        orthoProjection: true
        scene.activeCamera.cameraPreset: Camera3D.CameraPresetDirectlyAbove

        flipHorizontalGrid: false

        radialLabelOffset: 0.01

        horizontalAspectRatio: 2
        scene.activeCamera.zoomLevel: 160

        Surface3DSeries {
            id: surfaceSeries
            flatShadingEnabled: false
            drawMode: Surface3DSeries.DrawSurface
            baseGradient: surfaceGradient
            colorStyle: Theme3D.ColorStyleRangeGradient
            itemLabelFormat: "(@xLabel, @zLabel): @yLabel"

            ItemModelSurfaceDataProxy {
                itemModel: surfaceData.model
                rowRole: "x"
                columnRole: "y"
                yPosRole: "z"
            }
        }

    }

    Rectangle {
        id: legend
        anchors.margins: 5
        anchors.bottom: parent.bottom
        anchors.top:  parent.top
        anchors.right: parent.right
        border.color: "black"
        border.width: 1
        width: 10/root.dpi
        rotation: 180
        gradient: Gradient {
            GradientStop { position: 0.0; color: "black" }
            GradientStop { position: 0.2; color: "red" }
            GradientStop { position: 0.5; color: "blue" }
            GradientStop { position: 0.8; color: "yellow" }
            GradientStop { position: 1.0; color: "white" }
        }
    }

    Text {
        anchors.verticalCenter: legend.bottom
        anchors.right: legend.left
        anchors.margins: 2/root.dpi
        font.pixelSize: 7/root.dpi
        text: surfaceGraph.axisY.min  + "%"
    }

    Text {
        anchors.verticalCenter: legend.verticalCenter
        anchors.right: legend.left
        anchors.margins: 2/root.dpi
        font.pixelSize: 7/root.dpi
        text: (surfaceGraph.axisY.max + surfaceGraph.axisY.min) / 2  + "%"
    }

    Text {
        anchors.verticalCenter: legend.top
        anchors.right: legend.left
        anchors.margins: 2/root.dpi
        font.pixelSize: 7/root.dpi
        text: surfaceGraph.axisY.max + "%"
    }

}
