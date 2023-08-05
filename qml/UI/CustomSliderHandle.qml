import QtQuick 2.9


Rectangle {
    id: hoverRect
    radius: 14
    width: 28
    height: 28
    color: mouseArea.containsMouse ? "#d1d2d5" : "transparent"

    Rectangle {
        id: handle
        anchors.centerIn: parent
        height: 12
        width: height
        radius: height / 2
        color: root.enabled ? "#2d2f44" : "#bdbdbd"
    }

    //MouseArea {
    //    id: mouseArea
    //    anchors.fill: parent
    //    hoverEnabled: true
    //}
}

