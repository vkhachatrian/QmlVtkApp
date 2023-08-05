import QtQuick 2.9
import QtQuick.Controls
import QtQuick.Layouts 1.3
import QtQuick.Dialogs

import UI 1.0

Dialog {
    id: dialog
    anchors.centerIn: parent
    width: 440
    height: 164
    modal: true
    signal unloadClicked()
    property alias description: descriptionLabel.text
    property bool isMessageBox: false

    contentItem: Rectangle {
        anchors.centerIn: parent
        color: dialogBackground.color
        Label {
            id: descriptionLabel
            anchors.centerIn: parent
            font.pointSize: 14
        }
    }

    background: Rectangle {
        id: dialogBackground
        radius: 6
        color: "white"
    }

    header: Rectangle {
        radius: dialogBackground.radius
        color: dialogBackground.color
        visible: !isMessageBox
        Label {
            leftPadding: 32
            topPadding: 30
            text: "Unload Model"
            font.pointSize: 14
            color: "#2d2f44"
        }
    }

    footer:  Rectangle {
        width: parent.width
        height: 63
        radius: dialogBackground.radius
        color: dialogBackground.color
        RowLayout {
            anchors.fill: parent
            Item { // stretch
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            CustomButton {
                id: cancelButton
                width: 78
                height: 40
                buttonText: isMessageBox ? "Close" : "Cancel"
                borderWidth: 0
                onButtonClicked: dialog.reject()
            }
            CustomButton {
                id: unloadButton
                visible: !isMessageBox
                width: cancelButton.width
                height: cancelButton.height
                buttonText: "Unload"
                borderWidth: cancelButton.borderWidth
                onButtonClicked: unloadClicked()
            }
            Item { // stretch
                Layout.fillHeight: true
                width: 4
            }
        }
    }
}
