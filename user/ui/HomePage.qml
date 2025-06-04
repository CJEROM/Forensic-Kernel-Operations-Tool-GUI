import QtQuick
import QtQuick.Layouts
import QtCharts
import QtQuick.Controls

Page {
    id: pageHome

    LeftSideMenuBar {
        id: leftSideMenuBar
        width: 180
        height: parent.height
    }

    Rectangle {
        id: homePage
        anchors {
            left: leftSideMenuBar.right
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }

        // Component.onCompleted: {
        //     console.log(homePage.width)
        // }

        ScrollView {
            id: homeScroll
            anchors.fill: parent

            ColumnLayout {
                id: homeScrollColumn
                anchors.fill: parent
                anchors.margins: 10

                spacing: 10

                Rectangle {
                    id: title
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.preferredHeight: 80

                    Text {
                        text: "Dashboard"
                        anchors.centerIn: parent
                        font.bold: true
                        font.pixelSize: 30
                    }

                    color: "#ffffff"
                }


                //Filters: [Volume, MajorOperation, MinorOperation, ProcessPath, OpFileName, RequestorMode, TimeStart, TimeEnd]
                //Autosuggest for next path based on what is available in logs.

                //Build views that show unique items for specific rows, and help with contextualising into certain views.

                // Bar at top to show total statistics which are: Total Operations, Total Processes, Total Files,
                Item {
                    //width and height work correctly
                    id: content
                    Layout.fillWidth: true
                    property int adjustedHeight: (150 * (((content.width - 30) / 4) / 262))
                    Layout.preferredHeight: adjustedHeight
                    Layout.maximumHeight: 200

                    RowLayout {
                        //width and height dont work correctly
                        id: layout
                        anchors.fill: parent
                        // Layout.preferredHeight:

                        spacing: 10

                        Rectangle {
                            color: 'white'
                            Layout.preferredWidth: (parent.width - 30) / 4
                            Layout.fillHeight: true
                            Text {
                                id: mainStat1
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.leftMargin: 20
                                anchors.topMargin: 20
                                text: ">999,999" //The Actual stat
                                font.bold: true
                                font.pixelSize: 45
                            }
                            Text {
                                anchors.top: mainStat1.bottom
                                anchors.topMargin: 10
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                text: "Number of Operations" //What the actual stat mentions
                                font.pixelSize: 20
                            }
                        }

                        Rectangle {
                            color: 'white'
                            Layout.fillWidth: true
                            Layout.preferredWidth: (parent.width - 30) / 4
                            Layout.fillHeight: true
                            Text {
                                id: mainStat2
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.leftMargin: 20
                                anchors.topMargin: 20
                                text: ">999,999" //The Actual stat
                                font.bold: true
                                font.pixelSize: 45
                            }
                            Text {
                                anchors.top: mainStat2.bottom
                                anchors.topMargin: 10
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                text: "From User-Mode" //What the actual stat mentions
                                font.pixelSize: 20
                            }
                        }

                        Rectangle {
                            color: 'white'
                            Layout.fillWidth: true
                            Layout.preferredWidth: (parent.width - 30) / 4
                            Layout.fillHeight: true
                            Text {
                                id: mainStat3
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.leftMargin: 20
                                anchors.topMargin: 20
                                text: ">999,999" //The Actual stat
                                font.bold: true
                                font.pixelSize: 45
                            }
                            Text {
                                anchors.top: mainStat3.bottom
                                anchors.topMargin: 10
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                text: "Blocked Operations" //What the actual stat mentions
                                font.pixelSize: 20
                            }
                        }

                        Rectangle {
                            color: 'white'
                            Layout.fillWidth: true
                            Layout.preferredWidth: (parent.width - 30) / 4
                            Layout.fillHeight: true
                            Text {
                                id: mainStat4
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.leftMargin: 20
                                anchors.topMargin: 20
                                text: ">999,999" //The Actual stat
                                font.bold: true
                                font.pixelSize: 45
                            }
                            Text {
                                anchors.top: mainStat4.bottom
                                anchors.topMargin: 10
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                text: "Triggered Rules" //What the actual stat mentions
                                font.pixelSize: 20
                            }
                        }
                    }
                }

                Rectangle {
                    id: timeline1
                    color: 'white'
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300
                    Text {
                        anchors.centerIn: parent
                        text: parent.width + 'x' + parent.height
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300

                    RowLayout {
                        anchors.fill: parent
                        spacing: 10

                        // Most popular Operations
                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width - 10) / 2
                            color: "#222222"
                            id: stat1
                        }

                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width / 2) - 10
                            color: "#222222"
                            id: stat2
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300

                    RowLayout {
                        anchors.fill: parent
                        spacing: 10

                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width - 10) / 2
                            color: "#222222"
                            id: stat3
                            ScrollView {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                TableView {

                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width / 2) - 10
                            color: "#222222"
                            id: stat4
                            ScrollView {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                TableView {

                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }
                        }

                    }
                }


                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300

                    RowLayout {
                        anchors.fill: parent

                        spacing: 10

                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width - 10) / 2
                            color: "#222222"
                            ScrollView {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                TableView {
                                    id: stat5
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillHeight: true
                            Layout.preferredWidth: (parent.width / 2) - 10
                            color: "#222222"
                            ScrollView {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                TableView {
                                    id: stat6
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }
                        }

                    }
                }

                Rectangle {
                    id: timeline2
                    color: 'white'
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300
                    Layout.bottomMargin: 20
                    Text {
                        anchors.centerIn: parent
                        text: parent.width + 'x' + parent.height
                    }
                }

                //Most popular Operations (Piechart) + Table
                //Most popular Processes (Table)

                // ===== Line Graph to bring them together =====
                //Longest Operations time (Ranking Table)
                //Fastest Operations time
                //Average Operations Time

                //Number of User Operations
                //Number of Kernel Operations
                //Number of Operations (Bar Chart, Split between User and Kernel)

                //Most triggered Rules

                //Number of blocked Operations

                //Top Triggered Rules


                //      ======= Stats for Alert Page =====
                //Highest Triggered Rules

                // Component.onCompleted: {
                //     console.log(title.height + 20 + layout.height + timeline1.height + timeline2.height)
                //     console.log(title.height, 20 ,content.height, timeline1.height, timeline2.height)
                //     console.log(homeScrollColumn.height)
                //     console.log(homeScroll.height)
                //     console.log(homePage.height)
                // }
            }
        }

        color: "#CED4DA"
    }
}


