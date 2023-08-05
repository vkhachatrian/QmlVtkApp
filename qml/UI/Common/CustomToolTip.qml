import QtQuick 2.9
import QtQuick.Controls

Item {
    id: root

    property alias xPos: toolTip.x
    property alias yPos: toolTip.y
    property alias showed: toolTip.visible
    property alias toolTipText: toolTipBackgroundText.text

    ToolTip {
        id: toolTip

        background: Rectangle {
            id: back
            color: "white"
            border.color: "black"
            width: 28
            height: 25

            Label {
                id: toolTipBackgroundText
                anchors.centerIn: parent
            }
        }
    }

}
