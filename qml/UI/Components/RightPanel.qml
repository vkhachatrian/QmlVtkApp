import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import UI 1.0
import Instances 1.0

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 32
        spacing: 5

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 10

            Label {
                text: "Patient Name"
                font.family: "Poppins"
                font.pixelSize: 14
            }
            Image { source: "qrc:/images/logo_patient" }
        }

        CheckBoxWithSlider {
            labelText: "Maxillary"
            Layout.fillWidth: true
            onValueChanged: function(value) { VTKController.setMaxillaryOpacity(value) }
            onCheckBoxChecked: function(checked) { VTKController.setMaxillaryVisibility(checked) }
        }

        CheckBoxWithSlider {
            labelText: "Mandibular"
            Layout.fillWidth: true
            onValueChanged: function(value) { VTKController.setMandibularOpacity(value); }
            onCheckBoxChecked: function(checked) { VTKController.setMandibularVisibility(checked) }
        }

        Item {
            height: 82
            Layout.fillWidth: true
        }

        ViewButtonsGroup {
            height: 395
            width: 145
            Layout.alignment: Qt.AlignRight
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}

