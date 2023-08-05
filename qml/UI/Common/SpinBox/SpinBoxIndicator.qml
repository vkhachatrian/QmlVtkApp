import QtQuick 2.9

Item {
    id: root
    width: 40; height: 40
    signal clicked()
    property bool isUpButton: false

    Rectangle {
        anchors.fill: parent
        color: mouseArea.containsMouse ? "#eaebef" : "transparent"
        radius: 4

        Image {
            id: img
            anchors.centerIn:  parent
            source: isUpButton ? "qrc:/Icons/icon_spinbox_up" : "qrc:/Icons/icon_spinbox_down"
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: { root.clicked() }
        }
    }
}
