import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0

Item {
    id: root
    StackView {
        id: stack
        clip: true
        initialItem: segmentationSettings
        anchors.fill: parent
    }

    Component {
        id: segmentationSettings
        Item {
            SegmentationSettings {
                onCancelClicked: stack.push(teethSegmentation)
            }
        }
    }

    Component {
        id: teethSegmentation
        TeethSegmentation {
            width: root.width
            height: root.height
            onSettingsClicked: stack.pop()
        }
    }
}
