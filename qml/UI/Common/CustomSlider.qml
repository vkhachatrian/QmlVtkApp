import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: root
    property alias enabled: control.enabled
    property int handleSize: 12
    signal valueChanged(real value)

    Slider {
        id: control
        anchors.fill: parent
        value: 1
        from: 0
        to: 1

        background: Rectangle {
            x: control.leftPadding
            y: control.topPadding + control.availableHeight / 2 - height / 2
            height: 4
            width: control.availableWidth
            radius: height / 2
            color: root.enabled ? "#a8a9b1" : "#bdbdbd"

            Rectangle {
                width: control.visualPosition * parent.width
                height: parent.height
                color: root.enabled ? "#2d2f44" : "#dedede"
                radius: height / 2
            }
        }

        handle: Rectangle {
            id: handle
            x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
            y: control.topPadding + control.availableHeight / 2 - height / 2
            width: root.handleSize
            height: root.handleSize
            radius: height / 2
            color: root.enabled ? "#2d2f44" : "#bdbdbd"
        }
        onMoved: { root.valueChanged(value) }
    }
}


