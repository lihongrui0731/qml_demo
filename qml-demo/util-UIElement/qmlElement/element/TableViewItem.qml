import QtQuick 2.15
import QtQuick.Window 2.2
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 2.15

import "./../button"

Item {
    anchors.fill: parent
    anchors.margins: 20/root.dpi

    //[add header]
    function addColunm(rolorstr, titlestr)
        {
            var prefix = 'import QtQuick 2.7;import QtQuick.Controls 1.4;TableViewColumn {width: tableView.viewport.width/tableView.columnCount;';
            //创建TableViewColumn的代码
            //循环添加TableViewColumn
            var str = prefix  +"role:\"" + rolorstr + "\";title:\"" + titlestr + "\"}";
            tableView.addColumn(Qt.createQmlObject(str,tableView,"qrc:/TableViewItem.qml"));
        }
    //[!add header]

    //[addrowdata]
    function addRowData(d)
    {
         tablemode.append(d);
    }
    //[!addrowdata]

    ListModel {
        id: tablemode

        ListElement {
            fileName: "文件一12312312给一跟有2跟1g2g一2跟阿斯顿八十多该考生对啊啥都阿什顿"
        }
        ListElement {
            fileName: "文件二"
        }
        ListElement {
            fileName: "文件三"
        }

    }

    TableView{
        id :tableView
        anchors.fill: parent
        alternatingRowColors : false

        TableViewColumn {
              role: "fileName"
              title: "文件名"
              width: tableView.width - 232/root.dpi
        }
        TableViewColumn {
              role: "fileName"
              title: "操作"
              width: 230/root.dpi

              delegate: Rectangle{
                  color : styleData.selected ? "#f2f2f2": "#ffffff"
                  border.width:1
                  Item{
                      height:40/root.dpi
                      width: 160/root.dpi
                      anchors.centerIn: parent
                      ActionButton{
                          anchors.left: parent.left
                          anchors.top: parent.top
                          name:"查看"
                          w: 74/root.dpi
                          h: 40/root.dpi
                          onActionClicked: {
                            console.log(styleData.value )
                          }
                      }
                      ActionButton{
                          anchors.right: parent.right
                          anchors.top: parent.top
                          name:"删除"
                          w: 74/root.dpi
                          h: 40/root.dpi
                          onActionClicked: {
                            //删除
                              console.log( styleData.row )
                          }
                      }
                  }
              }
        }

        model: tablemode

        //自定义表头代理
        headerDelegate:
        Rectangle{
           //color: "#00498C"
           gradient: Gradient {
               GradientStop { position: 0.0; color: "#d7d7d7" }
               GradientStop { position: 1.0; color: "#f2f2f2" }
           }
           //color : styleData.selected ? "blue": "darkgray"
           width: 100/root.dpi
           height: 40/root.dpi
           border.color: "black"
           //radius: 5
           Text
            {
                anchors.centerIn : parent
                text: styleData.value
                font.pixelSize: 16/root.dpi
            }
       }

       //行代理可以修改行高等信息
       rowDelegate: Rectangle {
           height: 50/root.dpi
           color: "#ffffff"
           anchors.leftMargin: 1
       }

       itemDelegate: Rectangle{
           color : styleData.selected ? "#f2f2f2": "#ffffff"
           border.width:1
           Text {
                anchors.verticalCenter: parent.verticalCenter
                x:10/root.dpi
                color : styleData.selected ? "black": "black"
                text: styleData.value
                font.pixelSize: 14/root.dpi
           }
       }

       style: TableViewStyle{
             textColor: "black"
             highlightedTextColor : "#f2f2f2"  //选中的颜色
             backgroundColor : "#ffffff"
       }
    }

}
