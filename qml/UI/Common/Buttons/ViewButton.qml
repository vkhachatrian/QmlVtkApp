import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0

Item {
    id: root
    property bool  checked: false
    property alias iconSource: img.source
    property alias iconColor: img.color
    property alias promptText: promptLabel.text
    property alias promptLabelWidth: promptLabel.width
    property alias hovered: mouseArea.containsMouse
    property alias tooltTipText: toolTip.toolTipText
    signal buttonClicked

    implicitWidth: 140
    implicitHeight: 50

    RowLayout {
        anchors.fill: parent
        spacing: 6

        Label {
            id: promptLabel
            font.pixelSize: 14
            font.bold: true
            color: "#aeafba"
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
            visible: { (mouseArea.containsMouse || checked) }
            horizontalAlignment: Text.AlignRight
        }

        Rectangle {
            id: hoverRect
            height: 50
            width: root.implicitHeight
            Layout.alignment: Qt.AlignRight
            color: mouseArea.containsMouse ? ( checked ? "#b44fa3" : "#efeaef") : (checked ? "#a2238d" : "transparent")
            radius: 4

            CustomToolTip {
                id: toolTip
                xPos: mouseArea.mouseX + 10
                yPos: mouseArea.mouseY + 20
                showed: (hovered && !checked && toolTipText)
            }

            ColoredImage {
                id: img
                anchors.centerIn: parent
                icon.sourceSize: Qt.size(30, 30)
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: { buttonClicked() }
            }
        }
    }
}
