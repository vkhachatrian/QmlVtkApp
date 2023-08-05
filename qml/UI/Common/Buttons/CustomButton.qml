import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material.impl 2.12
import Qt5Compat.GraphicalEffects

import UI 1.0

Item {
    id: root

    //button
    property bool  filled: false
    property alias buttonText: contentText.text
    property color buttonColor: filled ? "#a2238d" : "white"
    property color buttonTextColor: !enabled ? "#e0e0e0" : filled ? "white" : "#a2238d"
    property int   borderWidth: filled ? 0 : 1
    property alias textFontSize: contentText.font.pixelSize
    property color hoverColor: filled ? "#601e4f" : "#fbf6fb"

    //icon
    property string iconSource
    property int   iconSize: 35

    property bool rightSideIcon: false

    signal buttonClicked
    width : 160
    height: 45

    Rectangle {
        id: customButton
        anchors.fill: parent
        border.width: root.borderWidth
        border.color: !enabled ? "#e0e0e0" : (mouseArea.containsMouse ? "#a2238d" : "#d091c6")
        radius: 4
        color: !enabled ? "white" : (mouseArea.containsMouse ? hoverColor : buttonColor)

        RowLayout {
            anchors.centerIn: parent
            spacing: 0
            layoutDirection: rightSideIcon ? Qt.RightToLeft : Qt.LeftToRight
            ColoredImage {
                id: buttonIcon
                icon.sourceSize: Qt.size(iconSize, iconSize)
                visible: source
                source: iconSource
                color: buttonTextColor
                rotation: !rightSideIcon && (iconSource.includes("right")) ? 180 : 0
            }

            Text {
                id: contentText
                font.pixelSize: 14
                font.family: "Poppins"
                font.bold: true
                color: buttonTextColor
            }
        }

        //Ripple {
        //    id: ripple
        //    clipRadius: 2
        //    width: parent.width
        //    height: parent.height
        //    pressed: mouseArea.pressed
        //    color: "#e0b7da"
        //    opacity: 0.2
        //    antialiasing: true
        //}
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: !enabled ? Qt.ArrowCursor : Qt.PointingHandCursor;
        onClicked: buttonClicked()
    }
}
