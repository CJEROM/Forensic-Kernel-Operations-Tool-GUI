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

        Item {
            id: options
            anchors {
                left: parent.left
                right: parent.right
                top: title.bottom
                margins: 10
            }

            height: 30

            RowLayout {
                anchors.fill: parent
                Layout.alignment: Qt.AlignHCenter
                spacing: 10

                Button {
                    width: 100
                    text: "Previous"
                    enabled: dbManager.currentPage > 0
                    onClicked: dbManager.currentPage = (dbManager.currentPage - 1)
                }

                Text {
                    width: 200
                    text: "Page " + (dbManager.currentPage + 1) + " of " + dbManager.totalPages
                }

                Button {
                    width: 100
                    text: "Next"
                    enabled: dbManager.currentPage + 1 < dbManager.totalPages
                    onClicked: dbManager.currentPage = (dbManager.currentPage + 1)
                }

                Button {
                    id: rowsPerPageButton
                    text: "Rows: " + dbManager.rowsPerPage

                    Menu {
                        id: rowsPerPageDropdownMenu
                        width: 200

                        MenuItem { text: "100"; onTriggered: dbManager.rowsPerPage = 100 }
                        MenuItem { text: "250"; onTriggered: dbManager.rowsPerPage = 250 }
                        MenuItem { text: "500"; onTriggered: dbManager.rowsPerPage = 500 }
                        MenuItem { text: "1000"; onTriggered: dbManager.rowsPerPage = 1000 }
                    }

                    onClicked: rowsPerPageDropdownMenu.open()
                }

                Button {
                    width: 100
                    text: "Refresh"
                    onClicked: dbManager.refreshQuery()
                }
            }
        }

        Rectangle {
            anchors {
                left: parent.left
                right: parent.right
                top: options.bottom
                bottom: parent.bottom
                margins: 10
            }

            clip: true
            color: '#ffffff'

            HorizontalHeaderView {
                id: logHeader
                syncView: tableView
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 40
                z:1
                clip: true

                anchors.rightMargin: vScroll.width

                delegate: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 40
                    color: "#eeeeee"
                    border.color: "#aaaaaa"

                    Text {
                        anchors.centerIn: parent
                        font.bold: true
                        text: dbManager.selectedRoles[column]
                    }
                }
            }

            // Header row
            TableView {
                id: tableView
                anchors.fill: parent
                model: dbManager.model
                rowSpacing: 1
                columnSpacing: 1

                ScrollBar.vertical: ScrollBar { id: vScroll; z:2 }
                ScrollBar.horizontal: ScrollBar { id: hScroll }

                topMargin: logHeader.height + 1
                rightMargin: vScroll.width + 1
                bottomMargin: hScroll.height + 1

                // Define column widths manually (since TableViewColumn is not available)
                columnWidthProvider: function(column) {
                    switch (dbManager.selectedRoles[column]) {
                        case "AlertID": return 80  // RuleAction
                        case "Timestamp": return 160  // RuleAction
                        case "AlertMessage": return parent.width - 244 - (vScroll.width + 1)  // RuleAction
                        default: return 80
                    }
                }

                delegate: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 32
                    border.color: "#cccccc"

                    Text {
                        anchors.centerIn: parent
                        text: model[dbManager.selectedRoles[column]]
                        font.pixelSize: 12
                        elide: Text.ElideRight
                    }
                }
            }

            Connections {
                target: dbManager
                onQueryHasRefreshed: tableView.forceLayout()
            }
        }

        color: "#CED4DA"
    }
}

