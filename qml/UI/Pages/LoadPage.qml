import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Dialogs
import QtQuick.Controls
import QtQml 2.3

import UI 1.0
import Instances 1.0

Rectangle {
    id: root
    signal loadingModelDone

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#e3e3e3"  }
        GradientStop { position: 0.10; color: "#f6f6f5" }
        GradientStop { position: 0.5; color: "#ffffff"  }
        GradientStop { position: 0.7; color: "#fdfdfc"  }
        GradientStop { position: 1; color: "#e6e6e6"  }
    }

    Connections {
        target: VTKController
        function onLoadingFinished() { loadingModelDone(); loader.sourceComponent = loaderButton }
    }

    Connections {
        target: AppController
        function onSearchingFileIsMissing(isMaxillary) {
            loader.sourceComponent = loaderButton
            if(isMaxillary)
                messageBox.description = "Maxillary is missing"
            else
                messageBox.description = "Mandibular is missing"

            messageBox.open();
        }
    }

    FolderDialog {
        id: fileDialog
        title: "Select folder to Upload"
        onAccepted: {
            loader.sourceComponent = progressCircle
            AppController.startObjectSearching(currentFolder.toString())
        }
    }

    Loader {
        id: loader
        anchors.centerIn: parent
        sourceComponent: loaderButton
    }

    Component {
        id: loaderButton
        CustomButton {
            anchors.centerIn: parent
            buttonText: "Load"
            filled: true
            onButtonClicked: fileDialog.visible = true
            textFontSize: 18
        }
    }

    Component {
        id: progressCircle
        ProgressCircle {
               value: 0.6
               secondaryColor: "transparent"
               primaryColor: "#a2238d"
           }
    }

    CustomDialog {
        id: messageBox
        isMessageBox: true
    }
}
