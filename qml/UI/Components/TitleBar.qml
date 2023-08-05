import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import UI 1.0

Rectangle {
    id: root
    height: 60

    property alias titleText: title.text
    property alias closeButtonVisible: closeButton.visible
    signal closeClicked

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 25
        anchors.rightMargin: closeButton.visible ? 25 : 25 + closeButton.width
        spacing: 0

        Image { source: "qrc:/images/appLogo.png" }

        Rectangle { // stretch
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Label {
            id: title
            //text: "Sculpt Maxillary"
            color: "#2d2f44"
            font.pixelSize: 22
            font.bold: true
            font.family: "Poppins"
            verticalAlignment: Text.AlignVCenter
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
        }

        Rectangle { // stretch
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        CustomButton {
            id: closeButton
            width: 130
            height: 40
            buttonText: "Close"
            onButtonClicked: { closeClicked() }
        }
    }
}
