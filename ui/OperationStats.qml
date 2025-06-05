import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Column {
    spacing: 10

    RowLayout {
        width: parent.width
        spacing: 10

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredWidth: 1
            height: 400
            color: "white"

            ChartView {
                anchors.fill: parent
                title: "Bar Chart 1"
                legend.alignment: Qt.AlignBottom
                antialiasing: true

                BarSeries {
                    axisX: BarCategoryAxis { categories: ["2007", "2008", "2009", "2010", "2011", "2012"] }
                    BarSet { label: "Bob"; values: [2, 2, 3, 4, 5, 6] }
                    BarSet { label: "Susan"; values: [5, 1, 2, 4, 1, 7] }
                    BarSet { label: "James"; values: [3, 5, 8, 13, 5, 8] }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredWidth: 1
            height: 400
            color: "white"

            ChartView {
                anchors.fill: parent
                title: "Bar Chart 2"
                legend.alignment: Qt.AlignBottom
                antialiasing: true

                BarSeries {
                    axisX: BarCategoryAxis { categories: ["A", "B", "C"] }
                    BarSet { label: "Sample"; values: [1, 2, 3] }
                }
            }
        }
    }

    Rectangle {
        width: parent.width
        height: 400
        color: "white"

        ChartView {
            anchors.fill: parent
            title: "Bar Chart 1"
            legend.alignment: Qt.AlignBottom
            antialiasing: true

            BarSeries {
                axisX: BarCategoryAxis { categories: ["2007", "2008", "2009", "2010", "2011", "2012"] }
                BarSet { label: "Bob"; values: [2, 2, 3, 4, 5, 6] }
                BarSet { label: "Susan"; values: [5, 1, 2, 4, 1, 7] }
                BarSet { label: "James"; values: [3, 5, 8, 13, 5, 8] }
            }
        }
    }

    Rectangle {
        width: parent.width
        height: 400
        color: "white"

        ChartView {
            anchors.fill: parent
            title: "Bar Chart 2"
            legend.alignment: Qt.AlignBottom
            antialiasing: true

            BarSeries {
                axisX: BarCategoryAxis { categories: ["A", "B", "C"] }
                BarSet { label: "Sample"; values: [1, 2, 3] }
            }
        }
    }
}
