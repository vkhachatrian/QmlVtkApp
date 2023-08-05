import QtQuick 2.9
import QtQuick.Controls 2.5

Item {
    id: root
    property bool checked: true
    property alias icon: img.source
    property alias iconSize: img.sourceSize
    property bool isViewIcon: false
    signal clicked(checked: bool)

    width:  35
    height: width

    Rectangle {
        anchors.fill: parent
        color: mouseArea.containsMouse ? "#efeaef" : "transparent"
        radius: 4

        Image {
            id: img
            anchors.centerIn: parent
            source: checked ? "qrc:/Icons/icon_spinBoxChecked" : "qrc:/Icons/icon_spinBoxUnChecked"
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                checked = !checked
                root.clicked(checked)
            }
        }
    }
}
