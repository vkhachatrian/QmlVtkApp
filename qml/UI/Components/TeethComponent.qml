import QtQuick 2.9

import UI 1.0
import Models 1.0

Item {
    id: root

    TeethModel { id: teethModel }

    Repeater {
        id: maxillary
        model: teethModel
        delegate: TeethWithNumber {
            index: $parameters.index
            number: $parameters.number
            numberInfo: $parameters.numberInfo
            teethInfo: $parameters.teethInfo
            teethState: $parameters.state
        }
    }



    CustomLabel {
        text: "Right"
        font.bold: true
        color: "black"
        x: 50; y: 265
    }

    CustomLabel {
        text: "Left"
        color: "black"
        font.bold: true
        x: 266.469; y: 265
    }

    CustomButton {
        id: resetMaxillary
        width: 125
        height: 50
        buttonText: "Reset"
        textFontSize: 18
        iconSize: 45
        enabled: false
        iconSource: "qrc:/Icons/icon_reload"
        onButtonClicked: { enabled = false; teethModel.resetStates(true); }
        x: 107.5
        y: 210
    }

    CustomButton {
        id: resetMandibular
        width: 125
        height: 50
        buttonText: "Reset"
        enabled: false
        textFontSize: 18
        iconSource: "qrc:/Icons/icon_reload"
        iconSize: 45
        onButtonClicked: { enabled = false; teethModel.resetStates(); }
        x: 107.500
        y: 315
    }
}
