import QtQuick 2.9
import QtQuick.Layouts 1.3

import VTK 9.2
import UI 1.0

Item {
    id: root
    property bool leftPanelEnabled: leftPanel.enabled
    property alias leftPanelActiveIndex: leftPanel.tabActiveItemIndex

    Column {
        anchors.fill: parent
        Rectangle {
            height: 12; width: parent.width
            color: "#e3e3e3"
        }

        RowLayout {
            width: parent.width
            height: parent.height - 12
            spacing: 0

            LeftPanel {
                id: leftPanel
                Layout.fillHeight: true
                Layout.preferredWidth: 380
                enabled: leftPanelEnabled
            }
            Item { // stretch
                Layout.fillHeight: true
                Layout.fillWidth: true

                VTKRenderWindow {
                    id: vtkWindow
                }
                VTKRenderItem {
                    anchors.fill:parent
                    objectName: "RenderItem"
                    renderWindow: vtkWindow
                }

                RightPanel {
                    anchors.right: parent.right
                    height: parent.height
                    width: 354
                }
            }
        }
    }



}
