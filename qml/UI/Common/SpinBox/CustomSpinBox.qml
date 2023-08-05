import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import UI 1.0

Item {
    id: root
    property int minValue: 0
    property int decimals: 2
    property real realValue: 10.0
    property real realFrom: 0.0
    property real realTo: 100.0
    property real realStepSize: 1.0
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        SpinBox {
            id: spinBox
            Layout.fillWidth: true
            Layout.fillHeight: true

            background: Rectangle {
                color: "#f6f5fa"
            }

            down.indicator: SpinBoxIndicator {
                id: downIndicator
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 5
                anchors.topMargin: 5
                onClicked: {
                    forceActiveFocus()
                    spinBox.value -= spinBox.stepSize
                }
            }
            up.indicator: SpinBoxIndicator {
                id: upIndicator
                isUpButton: true
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: 5
                anchors.topMargin: 5
                onClicked: {
                    forceActiveFocus()
                    spinBox.value += spinBox.stepSize
                }
            }
            property real factor: Math.pow(10, decimals)
            stepSize: realStepSize * factor
            value: realValue * factor
            to : realTo * factor
            from : realFrom * factor
            validator: DoubleValidator {
                bottom: Math.min(spinBox.from, spinBox.to)*spinBox.factor
                top:  Math.max(spinBox.from, spinBox.to)*spinBox.factor
            }

            textFromValue: function(value, locale) {
                return parseFloat(value * 1.0/factor).toFixed(decimals);
            }

            contentItem: TextField {
                id: spinBoxText
                background: Rectangle { color: "transparent" }
                anchors.left: downIndicator.right
                anchors.right: upIndicator.left
                verticalAlignment: Text.AlignVCenter
                validator: spinBox.validator
                text: spinBox.displayText
                font.pixelSize: 16
                //onTextChanged: {
                //    //spinBox.value = parseFloat(text).toFixed(decimals) * spinBox.factor;
                //}
            }
        }
        Rectangle {
            height: 1
            Layout.fillWidth: true
            color: spinBoxText.focus ? "#9c2288" : "#8e8e91"
        }
    }
}
