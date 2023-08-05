import QtQuick 2.9

Item {
    id: root
    property bool selected: false
    property alias buttonText: btnText.text
    property alias rightCornerVisible: rightWhiteCorner.visible
    signal clicked()
    height: 47
    enabled: parent.enabled

    Rectangle {
        anchors.fill: parent
        color: selected ? "white" : "#f2f6fa"
        border.color: selected ? "#dfe1ea" : "transparent"

        Rectangle {
            anchors.bottom: parent.bottom
            height: 1
            width: parent.width
            color: selected ? "white" :"#dfe1ea"
        }

        Rectangle {
            id: rightWhiteCorner
            anchors.right:  parent.right
            anchors.bottom: parent.bottom
            height: selected ? parent.height - 1 : parent.height
            width: 1
            color: selected ? "white" :"transparent"
            visible: false
        }

        Text {
            id: btnText
            anchors.centerIn: parent
            color: enabled ? (selected ? "#a2238d" : "#2d2f44") : "#97989a"
            font.pixelSize: selected ? 16 : 18
            font.bold: selected
            font.family: "Poppins"
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: { root.clicked() }
        }
    }
}

