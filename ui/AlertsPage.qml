import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    LeftSideMenuBar {
        id: leftSideMenuBar
    }

    Rectangle {
        id: alertsPage
        anchors {
            left: leftSideMenuBar.right
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }

        Rectangle {
            id: title
            height: 80

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 10
            }

            Text {
                text: "Alerts"

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }

                font.bold: true
                font.pixelSize: 30
            }
        }


        color: "#CED4DA"
    }
}

