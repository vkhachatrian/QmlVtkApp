import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import Models 1.0

import UI 1.0

Popup {
    padding: 4
    focus: true
    signal itemClicked(var params)
    property alias listView: view

    CameraParametersModel {
        id: cameraParametersModel
        isPositionView: false
    }

    background: Rectangle {
        color: "white"
        layer {
            enabled: true
            effect: DropShadow {
                cached: true
                radius: 18.0
                samples: 26
                color: "#d8d8d8"
                smooth: true
            }
        }
    }

    enter: Transition  {  NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 } }
    exit:  Transition  { NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 } }

    ListView {
        id: view
        model: cameraParametersModel
        interactive: false
        currentIndex: -1
        anchors.fill: parent
        focus: true
        delegate:
            CameraModeMenuItem {
            imageIcon: $parameters.icon
            itemText: $parameters.name
            tooltTipText: $parameters.toolTipText
            imageColor: checked ? "#a2238d" : "transparent"
            checked: view.currentIndex === index
            onClicked: { view.currentIndex = index; itemClicked($parameters); }
        }
    }
}
