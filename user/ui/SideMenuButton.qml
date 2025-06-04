import QtQuick 2.15

Rectangle {
    property var buttonSource
    property var buttonText
    property var buttonPageLink
    property var fontColour: "#343A40"

    width: 160
    height: 40

    color: "#DEE2E6"

    Image {
        id: buttonImageLabel
        source: buttonSource
        height: parent.height * 0.6
        fillMode: Image.PreserveAspectFit

        anchors {
            verticalCenter: parent.verticalCenter
            leftMargin: 20
            left: parent.left
        }
    }

    Rectangle {

    }

    Text {
        id: buttonTextLabel
        text: buttonText
        font.pixelSize: 15
        font.bold: true

        anchors {
            left: buttonImageLabel.right
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }

        color: fontColour
    }

    //Pages are changed by doing
    //stackView.push("[Name of new page QML file]")

    MouseArea {
        anchors.fill: parent
        onClicked: stackView.replace(buttonPageLink)
    }
}

