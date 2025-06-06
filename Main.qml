import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Dialogs

ApplicationWindow {
    id: mainApp
    minimumWidth: 1280
    minimumHeight: 720
    width: 1280
    height: 720
    visible: true
    title: qsTr("Kernel Gate")

    property var dbPath: ""

    Column {
        anchors.centerIn: parent
        spacing: 10

        Button {
            text: "Choose Database File"
            onClicked: dbFileDialog.open()
        }

        Label {
            text: dbPath !== "" ? "Selected: " + dbPath : "No database selected"
            wrapMode: Text.Wrap
            width: parent.width * 0.8
        }
    }

    FileDialog {
        id: dbFileDialog
        title: "Select SQLite Database"
        nameFilters: ["SQLite files (*.db)", "All files (*)"]
        fileMode: FileDialog.OpenFile

        onAccepted: {
            let localPath = selectedFile.toString().startsWith("file:///")
                                ? selectedFile.toString().replace("file:///", "")
                                : selectedFile.toString();

            dbPath = localPath

            console.log("Selected DB:", localPath)
            let result = dbManager.setDatabasePath(localPath);
            if (result === 1) {
                //Successfull database
                dataModelRegistry.refreshAllModels();
                stackView.push("qrc:/qt/qml/user/ui/HomePage.qml");
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }

        //Have text field that can help you navigate file system, and on click and successfult database connection, pushes home page onto the stack.

        // Component.onCompleted: push("qrc:/qt/qml/user/ui/HomePage.qml")
        // initialItem: "qrc:/qt/qml/user/ui/HomePage.qml"
    }
}
