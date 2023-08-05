import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import UI 1.0
import Instances 1.0

Item {
    id: root
    height: 47
    property int currentIndex: -1
    enabled: parent.enabled

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            width: 1
            height: parent.height
            color: "#e7e7e7"

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#dfe1ea"
            }
        }

        CustomTabBarButton {
            width: 112
            buttonText: "Plane Cut"
            selected: currentIndex === 0
            onClicked: { currentIndex = 0; VTKController.planeCutActivated();}
        }

        Rectangle {
            width: 4
            height: parent.height
            color: "#e7e7e7"

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#dfe1ea"
            }
        }

        CustomTabBarButton {
            width: 117
            buttonText: "Spline Cut"
            selected: currentIndex === 1
            onClicked: { currentIndex = 1; VTKController.splineCutActivated(); }
        }

        Rectangle {
            width: 4
            height: parent.height
            color: "#e7e7e7"
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: "#dfe1ea"
            }
        }

        CustomTabBarButton {
            width: 142
            buttonText: "Segmentation"
            selected: currentIndex === 2
            rightCornerVisible: true
            onClicked: { currentIndex = 2 }
        }
    }
}
