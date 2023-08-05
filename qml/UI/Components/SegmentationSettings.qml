import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0

TabContainer {
    title: "Segmentation Settings"
    signal cancelClicked

    ColumnLayout {
        spacing: 24

        LabelWithComboBox {
            labelText: "Teeth Numbering System"
            model: [ "Universal numbering system", "FDI two-digit notation", "Palmer notation" ]
        }

        Column {
            spacing: 3

            LabelWithComboBox {
                labelText: "Teeth Segmentation Mode"
                model: [ "Manual", "Automatic"]
            }

            CustomLabel { text: "Segmentation Mode cannot be changed\nafter step 1" }
        }

        Column {
            spacing: 3

            CustomLabel { text: "Limit Cervical Points" }

            CustomSpinBox {
                width: 150; height: 50
                minValue: 10
            }

            CustomLabel { text: "Limit cannot be changed after step 1. Min\nvalue is 10" }
        }

        RowLayout {
            Layout.fillWidth: true
            height: cancelButton.height
            spacing: 20
            Layout.topMargin: 16
            Layout.leftMargin: 10

            CustomButton {
                id: cancelButton
                buttonText: "Cancel"
                width: 150
                onButtonClicked: cancelClicked()
            }

            CustomButton {
                width: 150
                buttonText: "Save"
                filled: true
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
