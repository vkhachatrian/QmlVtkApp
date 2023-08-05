import QtQuick 2.9
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects

import UI 1.0

Item {
    id: root
    width: 163
    height: 42

    property alias imageIcon:    img.source
    property alias imageColor:   img.color
    property alias itemText:     label.text
    property alias tooltTipText: toolTip.toolTipText
    property bool  checked:      false
    signal clicked

    Rectangle {
        id: mainRect
        anchors.fill: parent
        color: mouseArea.containsMouse ? ( checked ? "#f4e4f1" : "#f5f5f5") : (checked ?  "#f8eef6" : "transparent")

        RowLayout {
            id: rowLayout
            anchors.fill: parent
            anchors.leftMargin: 20

            ColoredImage { id: img; icon.sourceSize: Qt.size(30, 30) }

            Text {
                id: label
                font.pixelSize: 14
                font.bold: checked
                font.family: "Poppins"
                color: checked ? "#a2238d" : "#777886"
                Layout.fillWidth: true
            }
        }

        CustomToolTip {
            id: toolTip
            xPos: mouseArea.mouseX + 10
            yPos: mouseArea.mouseY + 20
            showed: (mouseArea.containsMouse && toolTipText)
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
