import QtQuick 2.9
import QtQuick.Controls 2.5
import Qt5Compat.GraphicalEffects

import UI 1.0

Popup {
    id: popup
    padding: 4
    focus: true
    implicitHeight: 30 * view.count + 2 * padding
    implicitWidth: 114;

    property alias model: view.model
    property alias selectedItem: view.currentIndex
    signal itemClicked(var index)

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

    ListView {
        id: view
        interactive: false
        currentIndex: -1
        anchors.fill: parent
        focus: true
        delegate: CustomMenuItemDelegate {
            width: popup.width - 2 * popup.padding
            height: 30
            labelText: modelData
            onClicked: { popup.close(); popup.itemClicked(index); }
            checked: index === view.currentIndex
        }
    }
}
