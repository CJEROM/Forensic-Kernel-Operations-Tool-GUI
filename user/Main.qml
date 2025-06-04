import QtQuick
import QtQuick.Controls
import QtQuick.Window

ApplicationWindow {
    id: mainApp
    minimumWidth: 1280
    minimumHeight: 720
    width: 1280
    height: 720
    visible: true
    title: qsTr("Kernel Gate")

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

        Component.onCompleted: push("qrc:/qt/qml/user/ui/HomePage.qml")
        // initialItem: "qrc:/qt/qml/user/ui/HomePage.qml"
    }
}
