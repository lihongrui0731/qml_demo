import QtQuick 2.15

Item {
    id:gallery
    property int count: 1
    property string type: "row" // row || column
    property int space: 10
    width: parent.width
    height: parent.height

    Row{
        anchors.fill: parent
        visible: gallery.type == "row" ?true:false
        spacing: gallery.space
        Repeater{
            model: gallery.type == "row" ? gallery.count:0

            Rectangle{
                width: ( parent.width - ( gallery.count - 1) * gallery.space) / gallery.count
                height: parent.height
                Image {
                    width: parent.width
                    source: "qrc:/gallary/gallery"+(index+1)+".png"
                    fillMode:  Image.PreserveAspectFit
                    asynchronous: true
                }
            }
        }
    }

    Column{
        anchors.fill: parent
        visible: gallery.type == "column" ?true:false
        spacing: gallery.space
        Repeater{
            model: gallery.type == "column" ? gallery.count:0
            Rectangle{
                height: ( parent.height  - ( gallery.count -1 ) * gallery.space ) / gallery.count
                width: parent.width
                Image {
                    width: parent.width
                    source: "qrc:/gallary/gallery"+(index+1)+".png"
                    fillMode:  Image.PreserveAspectFit
                    asynchronous: true
                }
            }
        }
    }

}
