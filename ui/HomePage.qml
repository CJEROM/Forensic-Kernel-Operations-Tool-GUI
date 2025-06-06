import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Page {
    id: pageHome

    property int activeTab: 0  // Track current tab

    LeftSideMenuBar {
        id: leftSideMenuBar
        width: 180
        height: parent.height
    }

    ColumnLayout {
        anchors {
            left: leftSideMenuBar.right
            top: parent.top
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }

        spacing: 12

        // Dashboard title
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "white"
            Text {
                anchors.centerIn: parent
                text: "Dashboard"
                font.pixelSize: 24
                font.bold: true
            }
        }

        // Summary cards (same as before)
        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            height: 100

            Repeater {
                model: [
                    { title: "Number of Operations", value: dataModelRegistry.modelTotalOps.get(0).TotalOperations !== undefined ? dataModelRegistry.modelTotalOps.get(0).TotalOperations : 0 },
                    { title: "From User-Mode", value: dataModelRegistry.modelRequestorMode.get(1).Count !== undefined ? dataModelRegistry.modelRequestorMode.get(1).Count : 0},
                    { title: "Blocked Operations", value: 0 },
                    { title: "Alerts", value: dataModelRegistry.modelTotalAlerts.get(0).TotalAlerts !== undefined ? dataModelRegistry.modelTotalAlerts.get(0).TotalAlerts : 0}
                ]

                Rectangle {
                    Layout.fillWidth: true
                    height: 150
                    color: "white"
                    // radius: 6
                    // border.color: "#aaa"
                    Column {
                        anchors.centerIn: parent
                        spacing: 4
                        Text {
                            text: modelData.value
                            font.pixelSize: 18
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            width: parent.width
                        }

                        Text {
                            text: modelData.title
                            font.pixelSize: 13
                        }
                    }
                }
            }
        }

        // Tab buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button { text: "Operation Stats"; onClicked: activeTab = 0; Layout.fillWidth: true; Layout.preferredWidth: 1 }
            Button { text: "File Stats"; onClicked: activeTab = 1; Layout.fillWidth: true; Layout.preferredWidth: 1 }
            Button { text: "Process Stats"; onClicked: activeTab = 2; Layout.fillWidth: true; Layout.preferredWidth: 1 }
        }

        ScrollView {
            id: scrollArea
            Layout.fillWidth: true
            Layout.fillHeight: true

            Loader {
                id: tabLoader
                width: scrollArea.width - (tabLoader.height > scrollArea.height ? scrollArea.effectiveScrollBarWidth : 0)
                sourceComponent: activeTab === 0 ? operationStats :
                                 activeTab === 1 ? fileStats :
                                 processStats

                onLoaded: {
                    if (item) item.width = tabLoader.width;
                    console.log(item.width);
                }
            }
            // Add more pages

        }

        Component {
            id: operationStats
            OperationStats { width: tabLoader.width }
        }

        Component {
            id: fileStats
            OperationFileStats { width: tabLoader.width }
        }

        Component {
            id: processStats
            OperationProcessStats { width: tabLoader.width }
        }
    }
}
