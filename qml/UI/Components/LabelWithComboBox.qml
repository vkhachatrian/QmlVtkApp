import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import UI 1.0

Item {
    id: root
    width: 340
    height: 65
    property alias labelText: label.text
    property alias model: comboBox.model

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        CustomLabel {
            id: label            
        }

        CustomComboBox {
            id: comboBox
        }
    }
}

