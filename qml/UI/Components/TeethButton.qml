import QtQuick 2.9
import QtQuick.Controls 2.5
import Qt5Compat.GraphicalEffects

import UI 1.0

ColoredImage {
    id: teeth
    color: (mouseArea.containsMouse || menu.opened) ? "#aaf5f5f5" : "transparent"
    anchors.margins: 5

    property alias teethStatesList: menu.model
    property alias teethState: menu.selectedItem
    property alias teethMenu: menu
    signal stateChanged(var menuItemIndex)

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor;
        onClicked: { menu.open(); }
    }

    TeethMenu {
        id: menu
        onItemClicked: function(index) { teeth.stateChanged(index) }
    }
}
