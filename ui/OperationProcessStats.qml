import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Column {
    spacing: 10
    anchors.fill: parent

    ScrollView {
        id: tableWrapper
        anchors.fill: parent
        height: parent.height

        Column {
            id: tableScroll

            anchors.fill: parent

            HorizontalHeaderView {
                id: processStatTableHeaders
                syncView: processStatTable
                width: parent.width
                height: 40
                clip: true
                z: 1

                delegate: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 40
                    color: "#eeeeee"
                    border.color: "#aaaaaa"

                    Text {
                        anchors.centerIn: parent
                        font.bold: true
                        text: column === 0 ? "PID" :
                              column === 1 ? "Process Path" :
                                             "Operations"
                    }
                }
            }

            TableView {
                id: processStatTable
                model: dataModelRegistry.modelProcessCounts

                width: parent.width
                height: contentHeight

                clip: true
                reuseItems: true

                columnSpacing: 1
                rowSpacing: 1

                columnWidthProvider: function(column) {
                    switch (column) {
                        case 0: return 80;   // PID
                        case 1: return tableWrapper.width - 200;  // Path
                        case 2: return 120;  // Count
                        default: return 140;
                    }
                }

                delegate: Rectangle {
                    implicitWidth: 180; height: 32
                    color: "white"
                    border.color: "#cccccc"
                    Text {
                        anchors.centerIn: parent
                        text: column === 0 ? model.ProcessId :
                              column === 1 ? model.ProcessFilePath :
                                             model.Count
                        font.pixelSize: 12
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }

    onWidthChanged: tableScroll.forceLayout()
    onHeightChanged: tableScroll.forceLayout()
}
