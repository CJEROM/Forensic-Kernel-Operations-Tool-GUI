import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    LeftSideMenuBar {
        id: leftSideMenuBar
    }

    Rectangle {
        id: dataPage
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
                text: "Data"

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
                    width: 100
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
                    id: columnsDropdownButton
                    text: "Columns To Show"

                    Menu {
                        id: columnsDropdownMenu
                        width: 200

                        Repeater {
                            model: dbManager.selectedRoles()

                            MenuItem {
                                contentItem: CheckBox {
                                    text: modelData
                                    checked: dbManager.selectedRoles.indexOf(modelData) !== -1

                                    onCheckedChanged: {
                                        dbManager.toggleRole(modelData, checked)
                                    }
                                }
                            }
                        }
                    }

                    onClicked: columnsDropdownMenu.open()
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
                    // visible: dbManager.selectedRoles.indexOf(tableView.roleNameList[column]) !== -1 ? true : false

                    Text {
                        anchors.centerIn: parent
                        font.bold: true
                        text: dbManager.selectedRoles[column]
                    }
                }

                //Option where on clicking header, you get option to set sorting and filtering
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
                        case "LogID":  return 100   // LogID
                        case "SeqNum":  return 100   // SeqNum
                        case "OprType":  return 80  // OprType
                        case "PreOpTime":  return 160  // PreOpTime
                        case "PostOpTime":  return 160  // PostOpTime
                        case "ProcessId":  return 80   // ProcessId
                        case "ProcessFilePath":  return dbManager.computeColumnWidth(column, 12)  // ProcessFilePath
                        case "ThreadId":  return 80   // ThreadId
                        case "MajorOp":  return 300  // MajorOp
                        case "MinorOp":  return 250  // MinorOp
                        case "IrpFlags": return 80  // IrpFlags
                        case "DeviceObj": return 160  // DeviceObj
                        case "FileObj": return 160  // FileObj
                        case "FileTransaction": return 160  // FileTransaction
                        case "OpStatus": return 400  // OpStatus
                        case "Information": return 160  // Information
                        case "Arg1": return 100  // Arg1
                        case "Arg2": return 100  // Arg2
                        case "Arg3": return 100  // Arg3
                        case "Arg4": return 100  // Arg4
                        case "Arg5": return 100  // Arg5
                        case "Arg6": return 100  // Arg6
                        case "OpFileName": return dbManager.computeColumnWidth(column, 12)  // OpFileName
                        case "RequestorMode": return 100  // RequestorMode
                        case "RuleID": return 80   // RuleID
                        case "RuleAction": return 80  // RuleAction
                        default: return 80
                    }
                }

                delegate: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 32
                    // color: styleData.selected ? "#d0eaff" : "#ffffff"
                    border.color: "#cccccc"
                    // visible: dbManager.selectedRoles.indexOf(tableView.roleNameList[column]) !== -1 ? true : false

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
                // onTotalPagesChanged:
            }
        }

        color: "#CED4DA"
    }
}


