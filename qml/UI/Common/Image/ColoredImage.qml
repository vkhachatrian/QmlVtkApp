import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import Qt5Compat.GraphicalEffects

import UI 1.0

Item {
    id: control
    implicitHeight: icon.height
    implicitWidth: icon.width

    property alias icon: icon
    
    property color color: "transparent"
    property alias source: icon.source

    SvgImage {
        id: icon
        visible: !imgOverlay.visible
        asynchronous: true
        anchors.centerIn: parent
    }
    ColorOverlay {
        id: imgOverlay
        anchors.fill: icon
        source: icon
        color: control.color
        visible: !Qt.colorEqual(control.color, "transparent")
    }
}
