import QtQuick 2.9
import QtQuick.Controls 2.5

import UI 1.0

CustomLabel {
    id: numberLabel
    font.pixelSize: 12;
    font.bold: true;
    color: "black"

    property alias numberMenuItems: menu.model
    property alias numberPixelSize: numberLabel.font.pixelSize
    property alias numberMenu: menu

    signal numberChanged(var numberMenuIndex)

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: menu.open()
    }

    TeethMenu {
        id: menu
        implicitWidth: 60
        x: (numberMenuItems[0] >= 9 && numberMenuItems[0] <= 24) ? -implicitWidth - numberLabel.width : numberLabel.width
        onItemClicked: function(index){ numberChanged(index) }
    }
}
