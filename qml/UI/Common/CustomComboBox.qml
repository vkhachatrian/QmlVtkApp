import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects

import UI 1.0


ComboBox {
    id: control
    implicitHeight: 40
    implicitWidth: 340
    leftPadding: 12

    delegate: CustomMenuItemDelegate {
        labelText: modelData
        onClicked: { control.currentIndex = index; control.popup.close(); }
        checked: control.currentIndex === index
    }

    indicator: Image {
        id: img;
        source: "qrc:/Icons/icon_comboBox_down"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        rotation: popup.opened ? 180 : 0
    }

    contentItem: CustomLabel {
        text: control.displayText
        font.bold: true
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitHeight: control.height + line.height
        border.color: "transparent"
        color: "#f3f4f8"
        radius: 4

        Rectangle {
            id: line
            width: parent.width
            anchors.bottom: parent.bottom
            height: 2
            color: popup.opened ? "#9c2288" : "#8e8e91"
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor;
        }
    }

    popup: Popup {
        id: popup
        y: control.height + line.height
        width: control.width
        implicitHeight: 40 * control.count + 4
        padding: 4
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
        }

        background: Rectangle {
            border.color: "transparent"
            radius: 4
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
    }
}


