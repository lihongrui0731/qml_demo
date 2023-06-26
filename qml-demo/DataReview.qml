import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    Button {
        id: testButton
        text: "click"
        onClicked: {
            listModel.addItem()
            console.log(listModel.count)
        }
    }

    //    Component {
    //        id: filesListComponent
    ListView {
        id: filesList
        anchors.top: testButton.bottom
        width: 100
        height: 100
        spacing: 5
        model: listModel
        delegate: Rectangle {
            width: filesList.width
            height: 20
            color: "purple"
            Label {
                id: lable
                text: "Label: " + label

            }
        }
    }
    //    }
    ListModel {
        id: listModel
        ListElement {
            label: "aaaaaaa"
            value: "2.45"
        }
        function addItem() {
            listModel.append({label: "aaa", value: "bbbb"})
        }
    }
}
