import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0
import Instances 1.0

Item {
    TabContainer {
        title: "Plane Cut Settings"
        ColumnLayout {
            spacing: 32
            Row {
                spacing: 10

                Label {
                    text: "Smooth Radius"
                    font.family: "Poppins"
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 14
                    color: "#2d2f44"
                }

                CustomSpinBox { width: 150; height: 50 }
            }
            ButtonsGroup {
                Layout.fillWidth: true
                onRunButtonClicked: {PlaneCutController.runPlaneCut();}
                onSwapDirectionButtonClicked: {PlaneCutController.changeSwapDirection();}
                onClearButtonClicked: {PlaneCutController.clearCutPlane();}
            }
            Item { Layout.fillHeight: true }
        }
    }
}
