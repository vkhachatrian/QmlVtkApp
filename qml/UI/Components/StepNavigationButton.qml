import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0

Item {
    id: root
    width: 80; height: 30

    property bool checked: false
    property alias number: number.text
    property alias label: label.text
    property bool lastStep: false

    RowLayout {
        anchors.fill: parent
        spacing: 4

        Rectangle {
            id: circle
            width: checked ? 20 : 14; height: width
            color: checked ? "#a2238d" : "#dfe1ea"
            radius: height / 2
            visible: !lastStep

            CustomLabel {
                id: number
                anchors.centerIn: parent
                font.pixelSize: checked ? 14 : 9
                font.bold: checked
                color: "white"
            }

        }

        CustomLabel {
            id: label
            text: "Step"
            color: checked ? "#a2238d" : "#dfe1ea"
            anchors.rightMargin: 4
            font.pixelSize: checked ? 14 : 11
        }

        ColorImage {
            id: img;
            source: "qrc:/Icons/icon_arrow_right"
            color: checked ? "#a2238d" : "#dfe1ea"
            visible: !lastStep
        }
    }
}
