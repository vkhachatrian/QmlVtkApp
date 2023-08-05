import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls
import QtQuick.Layouts 1.3

import UI 1.0
import Instances 1.0

Window {
    id: root
    visible: true
    visibility: "Maximized"
    minimumHeight: 450
    minimumWidth: 750
    property bool modelLoaded: false

    Connections {
        target: VTKController
        function onChoosedModifyingObject(name) { titleBar.titleText = "Sculpt " + name;
            if(!mainPage.leftPanelEnabled) mainPage.leftPanelEnabled = true; }

    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            color: "#e3e3e3"
            Layout.fillWidth: true
            height: 1
        }

        TitleBar {
            id: titleBar
            Layout.fillWidth: true
            titleText: loadPage.visible ? "Load Model" : ""
            closeButtonVisible: mainPage.visible
            onCloseClicked: { dialog.open() }
        }

        LoadPage {
            id: loadPage
            Layout.fillHeight: true
            Layout.fillWidth:  true
            visible: !modelLoaded
            onLoadingModelDone: { titleBar.titleText = ""; modelLoaded = true }
        }

        MainPage {
            id: mainPage
            Layout.fillHeight: true
            Layout.fillWidth:  true
            visible: modelLoaded
			leftPanelEnabled: false
        }
    }

    CustomDialog {
        id: dialog
        description: "Are you sure you want to unload the model?"
        onUnloadClicked: { VTKController.unloadModel(); titleBar.titleText = "Load Model"; dialog.accept(); modelLoaded = false; mainPage.leftPanelEnabled = false; mainPage.leftPanelActiveIndex = -1 }
    }

}
