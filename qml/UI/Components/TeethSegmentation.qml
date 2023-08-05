import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0

Item {
    signal settingsClicked

    ColumnLayout {
        anchors.fill: parent
        spacing: 24

        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 8
            Layout.rightMargin: 7
            Layout.topMargin: 20
            spacing: 4

            StepNavigationButton { checked: true; number: "1" }
            StepNavigationButton { number: "2" }
            StepNavigationButton { number: "3" }
            StepNavigationButton { number: "4" }
            StepNavigationButton { label: "Done"; lastStep: true; width: 40 }

        }

        Item {
            Layout.preferredHeight: 550
            Layout.preferredWidth: 340
            Layout.leftMargin: 20

            CustomButton {
                x: 100
                y: 0
                height: 25
                buttonColor: "transparent"
                hoverColor: buttonColor
                buttonText: "Upper (Maxillary)"
                borderWidth: 0
                textFontSize: 16
                buttonTextColor: "black"
            }

            Rectangle {
                    y: 0
                    x: 310
                    width: 40; height: width
                    color: mouseArea.containsMouse ? (mouseArea.pressed ? "#cecece" : "#f5f5f5") : "transparent"
                    radius: height / 2

                    ColoredImage {
                        anchors.centerIn: parent
                        source: "qrc:/Icons/icon_settings"
                        icon.sourceSize: Qt.size(30, 30)
                        color: mouseArea.containsMouse ? "#707070" : "transparent"
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor;
                        onClicked: settingsClicked()
                    }
                }

            TeethComponent { }

            CustomButton {
                x: 90;
                y: 524
                height: 25
                buttonColor: "transparent"
                hoverColor: buttonColor
                buttonText: "Lower (Mandibular)"
                textFontSize: 16
                borderWidth: 0
                buttonTextColor: "black"
                Layout.alignment: Qt.AlignCenter
            }

        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 20
            Layout.margins: 35
            Layout.bottomMargin: 15
            CustomButton {
                buttonText: "Back"
                iconSource: "qrc:/Icons/icon_filledArrow_right"
                width: 150
                iconSize: 30
                textFontSize: 16
                enabled: false
            }

            CustomButton {
                width: 150
                buttonText: "Next"
                filled: true
                iconSource: "qrc:/Icons/icon_filledArrow_right"
                rightSideIcon: true
                iconSize: 30
                textFontSize: 16
            }
        }
    }
}

