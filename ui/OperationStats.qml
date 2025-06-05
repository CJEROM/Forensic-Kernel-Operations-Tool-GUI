import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Column {
    spacing: 10

    // RowLayout {
    //     height: 400; width: parent.width; spacing: 10

    //     ChartView {
    //         Layout.fillWidth: true
    //         Layout.fillHeight: true
    //         title: "Requestor Mode Count"
    //         legend.alignment: Qt.AlignBottom
    //         antialiasing: true

    //         BarSeries {
    //             id: requestorSeries
    //             BarSet {
    //                 label: "Mode"
    //                 values: modelRequestorMode.count > 0 ? modelRequestorMode.get(0).Count.split(",").map(Number) : []
    //             }
    //         }
    //     }

    //     ChartView {
    //         Layout.fillWidth: true
    //         Layout.fillHeight: true
    //         title: "Operation Status Count"
    //         legend.alignment: Qt.AlignBottom
    //         antialiasing: true

    //         BarSeries {
    //             id: opStatusSeries
    //             BarSet {
    //                 label: "Status"
    //                 values: modelOpStatus.count > 0 ? modelOpStatus.get(0).Count.split(",").map(Number) : []
    //             }
    //         }
    //     }
    // }

    RowLayout {
        height: 400; width: parent.width; spacing: 10

        ChartView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Requestor Mode Count"
            legend.alignment: Qt.AlignBottom
            antialiasing: true

            BarSeries {
                id: requestorSeries
                BarSet {
                    label: "Mode"
                    values: modelRequestorMode.count > 0 ? modelRequestorMode.get(0).Count.split(",").map(Number) : []
                }
            }
        }

        ChartView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Operation Status Count"
            legend.alignment: Qt.AlignBottom
            antialiasing: true

            BarSeries {
                id: opStatusSeries
                BarSet {
                    label: "Status"
                    values: modelOpStatus.count > 0 ? modelOpStatus.get(0).Count.split(",").map(Number) : []
                }
            }
        }
    }

    RowLayout {
        width: parent.width
        spacing: 10

        TableView {
            Layout.fillWidth: true
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            height: 300
            model: modelDurationSummary
            delegate: ItemDelegate {
                text: model.MajorOp + ", " + model.MinorOp + " | Avg: " + model.AvgDurationNano + " ns"
            }
        }

        TableView {
            Layout.fillWidth: true
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            height: 300
            model: modelBreakdown
            delegate: ItemDelegate {
                text: model.MajorOp + " / " + model.MinorOp + ": " + model.Count
            }
        }
    }
}
