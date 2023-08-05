import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import UI 1.0

Item {
    id: root
    property alias labelText: label.text
    signal valueChanged(real value)
    signal checkBoxChecked(bool checked)
    height: 35

    RowLayout {
        anchors.fill: parent
        spacing: 0

        CustomCheckBox { onClicked: function(checked) { slider.enabled = checked; checkBoxChecked(checked) } }

        Label {
            id: label
            Layout.fillWidth: true
            font.family: "Poppins"
            color: "#2d2f44"
            font.pixelSize: 14
        }

        CustomSlider {
            id: slider
            width: 200
            height: root.height
            onValueChanged: function(value) { root.valueChanged(value) }
        }
    }
}
