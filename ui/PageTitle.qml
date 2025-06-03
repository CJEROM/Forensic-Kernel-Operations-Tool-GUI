import QtQuick 2.15

Item {
    property var pageTitle

    Rectangle {
        height: 80

        Text {
            text: pageTitle

            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }

            font.bold: true
            font.pixelSize: 30
        }
    }
}
