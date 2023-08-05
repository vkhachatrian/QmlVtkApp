import QtQuick 2.9
import QtQuick.Controls 2.5

import UI

Item {
    id: root
    property alias labelText: label.text
    property bool  checked: false
    signal clicked
    width: 340; height: 40

    Rectangle {
        anchors.fill: parent
        color: mouseArea.containsMouse ? ( checked ? "#f4e4f1" : "#f5f5f5") : (checked ?  "#f8eef6" : "transparent")

        CustomLabel {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            font.bold: checked
            color: checked ? "#a2238d" : "#2d2f44"
            leftPadding: 16
            rightPadding: 16
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: root.clicked()
        }
    }
}
