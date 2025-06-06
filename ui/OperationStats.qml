import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Column {
    spacing: 10


    RowLayout {
        height: 400; width: parent.width; spacing: 10

        // Pie Chart for User vs Kernel Requested Operations
        ChartView {
            id: requestorPieChart
            title: "Kernel Requested vs User Requested Operations"
            titleFont.bold: true
            Layout.fillWidth: true
            Layout.preferredWidth: 1
            Layout.fillHeight: true
            legend.visible: true
            legend.alignment: Qt.AlignRight
            antialiasing: true

            PieSeries {
                id: requestorPieSeries
            }

            Component.onCompleted: {
                const model = dataModelRegistry.modelRequestorMode
                const count = model.rowCount()

                for (let i = 0; i < count; ++i) {
                    const row = model.get(i)
                    const label = row.RequestorMode
                    const value = Number(row.Count)

                    requestorPieSeries.append(label, value)

                    let slice = requestorPieSeries.at(i)
                    slice.labelVisible = true
                    slice.label = `${label}: ${value}`
                }
            }
        }

        ScrollView {
            id: tableWrapper
            Layout.preferredWidth: 600
            Layout.fillHeight: true

            Column {
                id: tableScroll

                anchors.fill: parent

                HorizontalHeaderView {
                    id: durationHeader
                    syncView: durationTable
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
                            text: column == 0 ? "Major Op" :
                                                column == 1 ? "Average Time (ms)" :
                                                "Max Time (ms)"
                        }
                    }
                }

                TableView {
                    id: durationTable
                    model: dataModelRegistry.modelDurationSummary

                    width: parent.width
                    height: contentHeight

                    clip: true
                    reuseItems: true

                    columnSpacing: 1
                    rowSpacing: 1

                    columnWidthProvider: function(column) {
                        switch (column) {
                            case 0:  return 300
                            default: return 140
                        }
                    }

                    delegate: Rectangle {
                        implicitWidth: 180; height: 32
                        color: "white"
                        border.color: "#cccccc"
                        Text {
                            anchors.centerIn: parent
                            text: column == 0 ? model.MajorOp :
                                                column == 1 ? (Number(model.AvgDurationNano) / 1e6).toFixed(3) + " ms" :
                                                (Number(model.MaxDurationNano) / 1e6).toFixed(3) + " ms"
                            font.pixelSize: 12
                            elide: Text.ElideRight
                        }
                    }
                }
            }
        }
    }

    RowLayout {
        height: 600; width: parent.width; spacing: 10

        // Stacked Bar: MajorOp by RequestorMode
        ChartView {
            id: stackedBarChart
            title: "Major Operations by Requestor Mode"
            titleFont.bold: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            antialiasing: true
            legend.visible: true
            legend.alignment: Qt.AlignBottom

            StackedBarSeries {
                id: stackedSeries
                BarSet { id: userSet; label: "UserMode"; }
                BarSet { id: kernelSet; label: "KernelMode"; }
            }

            BarCategoryAxis {
                id: stackedAxisX
                labelsAngle: 90
            }

            ValueAxis {
                id: stackedAxisY
                min: 0
            }

            Component.onCompleted: {
                const model = dataModelRegistry.modelMajorOpByMode
                const count = model.rowCount()

                const categoryList = []
                const mapUser = {}
                const mapKernel = {}
                let maxVal = 0

                for (let i = 0; i < count; ++i) {
                    const row = model.get(i)
                    if (!row) continue

                    const op = row.MajorOp.trim()
                    const mode = row.RequestorMode.trim()
                    const value = Number(row.Count)

                    if (!categoryList.includes(op)) categoryList.push(op)

                    if (mode === "User") mapUser[op] = value
                    if (mode === "Kernel") mapKernel[op] = value

                    maxVal = Math.max(maxVal, value)
                }

                stackedAxisX.categories = categoryList

                for (let op of categoryList) {
                    const u = mapUser[op] || 0
                    const k = mapKernel[op] || 0

                    userSet.append(u)
                    kernelSet.append(k)
                }

                stackedSeries.axisX = stackedAxisX
                stackedSeries.axisY = stackedAxisY
                stackedAxisY.max = Math.max(10, Math.ceil(maxVal * 1.2))
            }
        }
    }

    RowLayout {
        id: firstStat
        height: firstStat.width * 0.5; width: parent.width; spacing: 10

        ChartView {
            id: opBreakdownPieChart
            title: "IRP Operations Breakdown"
            titleFont.bold: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            legend.visible: false
            antialiasing: true

            // === Outer Donut: MinorOp ===
            PieSeries {
                id: outerDonut
                holeSize: 0.505  // matches innerDonut.pieSize
                size: 0.7
            }

            // === Inner Donut: MajorOp ===
            PieSeries {
                id: innerDonut
                holeSize: 0.3
                size: 0.5  // controls outer edge of inner ring
            }

            Component.onCompleted: {
                const model = dataModelRegistry.modelBreakdown
                const count = model.rowCount()

                const majorTotals = new Map()
                const minorGrouped = {}

                // === Group by Major ===
                for (let i = 0; i < count; ++i) {
                    const row = model.get(i)
                    const major = row.MajorOp
                    const minor = row.MinorOp
                    const value = Number(row.Count)

                    // Add to major total
                    majorTotals.set(major, (majorTotals.get(major) || 0) + value)

                    // Store minor values per major
                    if (!minorGrouped[major])
                        minorGrouped[major] = []
                    if (minor && minor !== "")
                        minorGrouped[major].push({ minor, value })
                }

                // === Inner Donut: MajorOp slices ===
                majorTotals.forEach((total, major) => {
                    const slice = innerDonut.append(major, total)
                    slice.label = `${major}: ${total}`
                    slice.labelVisible = false
                    slice.labelPosition = PieSlice.LabelOutside

                    slice.clicked.connect(() => {
                        for (let j = 0; j < innerDonut.count; ++j)
                            innerDonut.at(j).labelVisible = false
                        for (let j = 0; j < outerDonut.count; ++j)
                            outerDonut.at(j).labelVisible = false

                        slice.labelVisible = true
                    })
                })

                // === Outer Donut: MinorOp slices ===
                for (let major of Array.from(majorTotals.keys())) {
                    const group = minorGrouped[major]

                    if (!group || group.length === 0) {
                        const slice = outerDonut.append("", majorTotals.get(major))
                        slice.labelVisible = false
                        slice.color = Qt.rgba(0, 0, 0, 0)
                        continue
                    }

                    for (const { minor, value } of group) {
                        const label = `${minor}`
                        const slice = outerDonut.append(label, value)
                        slice.label = `${label}: ${value}`
                        slice.labelVisible = false

                        slice.clicked.connect(() => {
                            for (let j = 0; j < innerDonut.count; ++j)
                                innerDonut.at(j).labelVisible = false
                            for (let j = 0; j < outerDonut.count; ++j)
                                outerDonut.at(j).labelVisible = false

                            slice.labelVisible = true
                        })
                    }
                }
            }
        }
    }

    RowLayout {
        height: parent.width * 0.55; width: parent.width; spacing: 10

        // Bar Chart for Operation Status Frequency
        ChartView {
            id: opStatusPieChart
            title: "Operation Status Breakdown"
            titleFont.bold: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            legend.visible: true
            legend.alignment: Qt.AlignRight
            antialiasing: true

            PieSeries {
                id: opStatusPieSeries

                onClicked: (slice) => {
                    opStatusPieSeries.remove(slice)
                }
            }

            function rebuildPieChart() {
                opStatusPieSeries.clear()

                const model = dataModelRegistry.modelOpStatus
                const count = model.rowCount()

                let unknownTotal = 0

                for (let i = 0; i < count; ++i) {
                    const row = model.get(i)
                    if (!row) continue

                    const label = row.OpStatus
                    const value = Number(row.Count)

                    if (label.startsWith("Unknown NTSTATUS")) {
                        unknownTotal += value
                        continue
                    }

                    const slice = opStatusPieSeries.append(label, value)
                    slice.labelVisible = true
                    slice.label = `${label}: ${value}`
                }

                if (unknownTotal > 0) {
                    const unknownSlice = opStatusPieSeries.append("Unknown NTSTATUS (combined)", unknownTotal)
                    unknownSlice.labelVisible = true
                    unknownSlice.label = `Unknown NTSTATUS (combined): ${unknownTotal}`
                }
            }

            Component.onCompleted: rebuildPieChart()
        }

        Button {
            text: "Reset Chart"
            width: 120
            onClicked: {
                opStatusPieChart.rebuildPieChart()
            }
        }
    }
}
