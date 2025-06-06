import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Column {
    spacing: 10
    id: tableScroll

    anchors.fill: parent

    HorizontalHeaderView {
        id: fileStatTablenHeaders
        syncView: fileStatTable
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
                text: column === 0 ? "OpFileName" : "Operations"
            }
        }
    }

    TableView {
        id: fileStatTable
        model: dataModelRegistry.modelFileCounts

        width: parent.width
        height: contentHeight

        clip: true
        reuseItems: true

        columnSpacing: 1
        rowSpacing: 1

        columnWidthProvider: function(column) {
            switch (column) {
                case 0: return tableScroll.width - 140;  // OpFileName
                case 1: return 120;  // Count
                default: return 140;
            }
        }

        delegate: Rectangle {
            implicitWidth: 180; height: 32
            color: "white"
            border.color: "#cccccc"
            Text {
                anchors.centerIn: parent
                text: column === 0 ? model.OpFileName : model.Count
                font.pixelSize: 12
                elide: Text.ElideRight
            }
        }
    }

    Component.onCompleted: {
        tableScroll.forceLayout()
    }

    onWidthChanged: tableScroll.forceLayout()
    onHeightChanged: tableScroll.forceLayout()
}

