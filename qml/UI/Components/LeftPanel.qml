import QtQuick 2.9
import QtQuick.Controls
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects

import UI 1.0

Item {
    id: root
    property alias tabActiveItemIndex: tabBar.currentIndex

    Rectangle {
        id: tabs
        anchors.fill: parent;

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            CustomTabBar { id: tabBar }

            StackLayout {
                width: parent.width
                currentIndex: tabBar.currentIndex

                PlaneCutTab { id: planeTab }

                SplineCutTab { id: splineTab }

                SegmentationTab { id: segmentationTab }
            }
        }
    }

    DropShadow {
        anchors.fill: tabs
        cached: true
        horizontalOffset: 3
        radius: 8.0
        samples: 16
        color: "#e7e7e7"
        smooth: true
        source: tabs
    }
}
