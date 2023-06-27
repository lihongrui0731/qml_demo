import QtQuick 2.15
import Qt.labs.platform 1.1
import QtQuick.Controls 2.15
import QtCharts 2.3
import QtQml.Models 2.15

import 'qrc:/js/color.js' as UI_color
import 'qrc:/js/config.js' as UI_config
import '../../resources/js/public.js' as JS

Item {
    id: chart
    anchors.fill: parent
    clip: true;

    property string channelId: ""
    property int currentChannel: -1
    property var categories: [];
    property var values:[];

    signal chooseChannel( var index_ );

    onChannelIdChanged: {
        chart.currentChannel = chart.categories.indexOf(chart.channelId);
        var val = chart.values[chart.currentChannel];
        chart.setValues( chart.channelId , val )
    }

    function clear(){
        axisx.clear();
        chart.categories = [];
        chart.values = [];
        value_.values = chart.values
        chart.currentChannel = -1
    }

    function addCategories( cat ){
        chart.categories.push(cat);
        chart.values.push(0);
        axisx.categories = chart.categories
        value_.values = chart.values
    }

    function setValues( cat , val ){
        var index_ = chart.categories.indexOf(cat);
        chart.values[index_] = val;

        var nochoose_ = new Array(chart.values.length).fill(0);
        var choose_ = new Array(chart.values.length).fill(0);
        for( var i = 0; i < chart.values.length; i++ ){
             if( i == chart.currentChannel && chart.currentChannel != -1  ){
                 choose_[i] = chart.values[i];
             }else{
                 nochoose_[i] = chart.values[i];
             }

        }

        value_.values = nochoose_
        chooseValue_.values = choose_
    }

    ChartView {
        id: chartView
        anchors.fill: parent
        anchors.margins: 0 / root.dpi
        antialiasing: false
        backgroundColor: "transparent"
        legend.visible : false
        margins.left: 0
        margins.right: 0
        margins.top: 0
        margins.bottom: 0

        BarCategoryAxis {
            id: axisx
            color: UI_color[UI_config[root.usedConfig].colorType].font_color
            labelsFont.pixelSize: 12/root.dpi
            labelsFont.bold: true
            labelsAngle: 90
            categories: chart.categories
        }

        ValueAxis {
            id: axisy
            max: 120
            min: 0
            tickCount: 7				//上图只是演示这个的功能，并非一般这么写，y轴方向7个指示点
            labelsColor: UI_color[UI_config[root.usedConfig].colorType].font_color
            labelsFont.pixelSize: 12/root.dpi
            labelsFont.bold: false
            labelFormat: "%d"
        }

        StackedBarSeries {
               id: barSeries
               axisX: axisx
               axisY: axisy
               labelsPosition: AbstractBarSeries.LabelsInsideEnd		//指条形图里面的数字值的位置
               labelsVisible: true										//指条形图里面的数字值是否显示，默认不显示
               useOpenGL: true
               BarSet{
                   id: value_
                   label: "设备编号"
                   color: UI_color[UI_config[root.usedConfig].colorType].home_title
                   borderColor: UI_color[UI_config[root.usedConfig].colorType].home_title
                   values: chart.values
                   onClicked: {
                       chart.currentChannel = index
                       // This is available in all editors.
                       chart.setValues( chart.categories[index] , chart.values[index] )

                       chart.chooseChannel( index )
                       console.log( index );
                   }
               }
               BarSet{
                   id: chooseValue_
                   label: "设备编号"
                   color: UI_color[UI_config[root.usedConfig].colorType].main_left
                   borderColor: UI_color[UI_config[root.usedConfig].colorType].main_left
                   values: []
               }

         }
    }

}
