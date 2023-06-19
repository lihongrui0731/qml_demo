import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: rangeBar
    color: "#eeeeee"
    width: parent.width
    height: parent.height

    property real warningLimit: 1
    property real dangerLimit: 5
    property real border_ : 2 / root.dpi
    property real value: 2

    onValueChanged: {
        rangeBar.getResult()
    }

    function getResult(){
        low.value = 0;
        normal.value = 0;
        high.value = 0;

        if( rangeBar.value >= 0 && rangeBar.value < rangeBar.warningLimit ){
            low.value = 1
        }else if( rangeBar.value >= rangeBar.warningLimit ){
            if( high.visible ){
                if( rangeBar.value < rangeBar.dangerLimit ){
                    normal.value = 1
                }else{
                    high.value = 1
                }
            }else{
                normal.value = 1
            }
        }
    }

    Row{
        id: row
        x: rangeBar.border_
        y: rangeBar.border_
        spacing: rangeBar.border_
        property real row_: high.visible?3:2
        ProgressBar{
            id: low
            value: 0
            height: rangeBar.height - rangeBar.border_*2
            width: ( rangeBar.width - rangeBar.border_*(2+row.row_-1) ) / (row.row_ -1 )
            background: Rectangle{
                color: "#009900"
                opacity: 0.1
            }

            contentItem: Item {
                Rectangle {
                    width: low.visualPosition * low.width
                    height: low.height
                    color: "green"
                }
            }

        }
        ProgressBar{
            id: normal
            value: 0
            height: rangeBar.height - rangeBar.border_*2
            width: ( rangeBar.width - rangeBar.border_*(2+row.row_-1) ) / (row.row_ -1 )
            background: Rectangle{
                color: "yellow"
                opacity: 0.1
            }
            contentItem: Item {
                Rectangle {
                    width: normal.visualPosition * normal.width
                    height: normal.height
                    color: "yellow"
                }
            }
        }
        ProgressBar{
            id: high
            value: 0
            height: rangeBar.height - rangeBar.border_*2
            width: ( rangeBar.width - rangeBar.border_*(2+row.row_-1) ) / (row.row_ -1 )
            visible: false
            background: Rectangle{
                color: "red"
                opacity: 0.1
            }
            contentItem: Item {
                Rectangle {
                    width: high.visualPosition * high.width
                    height: high.height
                    color: "red"
                }
            }
        }
    }
}
