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



            Rectangle {
                anchors.fill: parent
                color: '#ffffff'

                ToolButton {
                    id: dropdownButton
                    text: "Columns To Show"

                    Menu {
                        id: dropdownMenu
                        width: 200

                        Repeater {
                            model: tableView.roleNameList

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

                    onClicked: dropdownMenu.open()
                }

                // ToolButton {
                //     id: refreshButton
                //     text: "Columns To Show"

                //     onClicked: dropdownMenu.open()
                // }
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
                        text: tableView.roleNameList[column]
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

                property var roleNameList: [
                    "LogID",
                    "SeqNum",
                    "OprType",
                    "PreOpTime",
                    "PostOpTime",
                    "ProcessId",
                    "ProcessFilePath",
                    "ThreadId",
                    "MajorOp",
                    "MinorOp",
                    "IrpFlags",
                    "DeviceObj",
                    "FileObj",
                    "FileTransaction",
                    "OpStatus",
                    "Information",
                    "Arg1",
                    "Arg2",
                    "Arg3",
                    "Arg4",
                    "Arg5",
                    "Arg6",
                    "OpFileName",
                    "RequestorMode",
                    "RuleID",
                    "RuleAction"
                ]

                // Define column widths manually (since TableViewColumn is not available)
                columnWidthProvider: function(column) {
                    switch (column) {
                        case 0:  return 60   // LogID
                        case 1:  return 60   // SeqNum
                        case 2:  return 80  // OprType
                        case 3:  return 160  // PreOpTime
                        case 4:  return 160  // PostOpTime
                        case 5:  return 80   // ProcessId
                        case 6:  return dbManager.computeColumnWidth(6, 12)  // ProcessFilePath
                        case 7:  return 80   // ThreadId
                        case 8:  return 300  // MajorOp
                        case 9:  return 250  // MinorOp
                        case 10: return 80  // IrpFlags
                        case 11: return 160  // DeviceObj
                        case 12: return 160  // FileObj
                        case 13: return 160  // FileTransaction
                        case 14: return 400  // OpStatus
                        case 15: return 160  // Information
                        case 16: return 100  // Arg1
                        case 17: return 100  // Arg2
                        case 18: return 100  // Arg3
                        case 19: return 100  // Arg4
                        case 20: return 100  // Arg5
                        case 21: return 100  // Arg6
                        case 22: return dbManager.computeColumnWidth(22, 12)  // OpFileName
                        case 23: return 100  // RequestorMode
                        case 24: return 80   // RuleID
                        case 25: return 80  // RuleAction
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
                        text: model[tableView.roleNameList[column]]
                        font.pixelSize: 12
                        elide: Text.ElideRight
                    }
                }


            }
        }

        color: "#CED4DA"
    }
}


