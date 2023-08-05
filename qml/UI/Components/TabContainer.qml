import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0

ColumnLayout {
    anchors.fill: parent
    anchors.topMargin: 20
    anchors.leftMargin: 15
    anchors.rightMargin: 15
    spacing: 16

    default property alias _content: content.data
    property alias title: title.text

    CustomLabel {
        id: title
        font.pixelSize: 20
        font.bold: true
    }

    Item {
        id: content
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
