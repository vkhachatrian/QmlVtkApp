import QtQuick 2.9

import UI 1.0

Item {

    property int index: -1
    property var numberInfo
    property var teethInfo
    property alias teethState: teeth.teethState
    property alias number:  number.text


    TeethNumberButton {
        id: number
        x: numberInfo.x
        y: numberInfo.y
        numberMenuItems: numberInfo.menu
        numberPixelSize: (teeth.teethMenu.opened || number.numberMenu.opened) ? 16 : 12
        onNumberChanged: function(numberMenuIndex){ teethModel.setTeethNumber(index, numberMenuIndex) }
    }

    TeethButton {
        id: teeth
        x: teethInfo.x
        y: teethInfo.y
        teethStatesList: teethInfo.menu
        source: teethInfo.icon
        onStateChanged: function(menuItemIndex){
            if(index > 0 && index < 17)
                resetMaxillary.enabled = true;
            else
                resetMandibular.enabled = true;
            teethModel.setState(index, menuItemIndex)
        }
        teethMenu.x: (index >= 9 && index <= 24) ? -114 : teeth.width
    }
}
