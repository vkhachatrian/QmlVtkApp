import QtQuick 2.9
import QtQuick.Layouts 1.3

import UI 1.0

Item {
    id: root
    signal clearButtonClicked()
    signal swapDirectionButtonClicked()
    signal runButtonClicked()
    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        CustomButton {
            id: swapButton
            Layout.fillWidth: true
            height: 50
            buttonText: "Swap cut direction"
            onButtonClicked:{swapDirectionButtonClicked()}
        }

        RowLayout {
            spacing: 30
            CustomButton {
                buttonText: "Clear"            
                onButtonClicked:{clearButtonClicked()}
            }
            CustomButton {
                buttonText: "Run"
                filled: true
                iconSource: "qrc:/Icons/icon_play_white"
                onButtonClicked:{runButtonClicked()}
            }
        }
    }
}
