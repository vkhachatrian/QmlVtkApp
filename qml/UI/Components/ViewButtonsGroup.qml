import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import Models 1.0
import Instances 1.0
import UI 1.0

Item {
    id: root
    property int currentIndex: -1
    focus: true

    CameraParametersModel {
        id: cameraParametersModel
        isPositionView: true
    }

    RightPanelShortcuts {
        onMenuItemShortcutActivated: function(index){ menu.listView.currentIndex = index; VTKController.switchCameraMode(index) }
        onViewButtonShortcutActivated: function(index) { root.currentIndex = index; VTKController.switchView(index) }
    }

    ColumnLayout {
        id: column
        anchors.fill: parent

        Repeater {
            id: viewButtonsRepeater
            model: cameraParametersModel
            delegate: ViewButton {
                id: frontButton
                iconSource: checked ? $parameters.iconActive : $parameters.icon
                checked: currentIndex === index
                promptText: $parameters.promptText
                onButtonClicked: { currentIndex = index; VTKController.switchView(currentIndex) }
                tooltTipText: $parameters.toolTipText
            }
        }

        ViewButton {
            id: rotation
            iconSource: "qrc:/Icons/icon_rotate"
            //checked: currentIndex === 6
            promptText: "Camera\n  Mode"
            onButtonClicked: {
                if(menu.visible) {
                    menu.close()
                    return
                }
                menu.open()
                menu.forceActiveFocus()
            }
            onHoveredChanged: { menu.closePolicy = hovered ? Popup.NoAutoClose : (Popup.CloseOnEscape | Popup.CloseOnPressOutside) }
        }
    }

    CameraModeMenu {
        id: menu
        x: rotation.x - (width - rotation.promptLabelWidth)
        y: rotation.y - ((2 * rotation.height) + (2 * column.spacing))
        width: 171
        height: 262
        focus: true
        onItemClicked: function(params){
            menu.close()
            rotation.checked = true
            rotation.promptText = params.name
            rotation.iconSource = params.icon
            rotation.iconColor = "white"
            VTKController.switchCameraMode(listView.currentIndex)
        }
    }
}
