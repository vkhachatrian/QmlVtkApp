import QtQuick 2.0
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects

Item {
    width: 350; height: 50
    property bool isLinear: true
    signal modeButtonClicked(bool isLinear)

    Rectangle {
        id: mainRect
        anchors.fill: parent
        color: "#f4f5f9"
        radius: 4

        RowLayout {
            anchors.fill: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.leftMargin: 57
            anchors.rightMargin: 58
            z: 1

            Image {
                Layout.alignment: Qt.AlignLeft
                source: isLinear ? "qrc:/Icons/icon_splineSharpActive" : "qrc:/Icons/icon_splineSharp"
            }
            Image {
                Layout.alignment: Qt.AlignRight
                source: isLinear ? "qrc:/Icons/icon_splineCurve" : "qrc:/Icons/icon_splineCurveActive"
            }
        }

        Rectangle {
            id: moveRectangle
            width: mainRect.width / 2
            height: mainRect.height
            color: "#a2238d"
            radius: mainRect.radius

            SequentialAnimation on x {
                id: leftRightAnimation
                running: false
                PropertyAnimation {
                    from: isLinear ? mainRect.x : mainRect.width / 2
                    to: isLinear ? mainRect.width / 2 : mainRect.x
                    duration: 200
                    running: false
                }
            }
        }

        DropShadow {
            anchors.fill: moveRectangle
            cached: true
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 16
            color: "#d4cfd4"
            smooth: true
            source: moveRectangle
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                leftRightAnimation.running = true
                isLinear = (moveRectangle.x == mainRect.width / 2)
                modeButtonClicked(isLinear);
            }
        }
    }
}
