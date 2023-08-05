import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0
import Instances 1.0

Item {
    TabContainer {
        title: "Spline Cut Settings"

        ColumnLayout {
            spacing: 24

            ColumnLayout {
                spacing: 16
                Label {
                    text: "Spline Type"
                    font.pixelSize: 14
                    font.family: "Poppins"
                    color: "#2d2f44"
                }

                CustomSwitch {
                    Layout.fillWidth: true
                    height: 50
                    onModeButtonClicked : {SplineCutController.cumulativeModeActivated(!isLinear)}
                }
            }
            Row {
                spacing: 10

                Label {
                    text: "Fillet Radius"
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 14
                    font.family: "Poppins"
                    color: "#2d2f44"
                }
                CustomSpinBox {
                    width: 150
                    height: 50
                }
            }

            ButtonsGroup {
                Layout.fillWidth: true
                onRunButtonClicked: {}
                onSwapDirectionButtonClicked: {}
                onClearButtonClicked: {SplineCutController.clearCutSpline();}
            }
            Item { Layout.fillHeight: true }
        }
    }
}
