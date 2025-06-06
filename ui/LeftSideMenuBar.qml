import QtQuick 2.15

Rectangle {
    id: leftSideMenuBar
    height: parent.height
    width: 180
    color: "#F8F9FA"

    SideMenuButton {
        id: homePageButton
        buttonSource: "qrc:/qt/qml/user/assets/img/home.png"
        buttonPageLink: "qrc:/qt/qml/user/ui/HomePage.qml"
        buttonText: "Home"
        dataTable: "MinifilterLog"

        anchors {
            left: parent.left
            leftMargin: 10
            top: parent.top
            topMargin: 10
        }
    }

    SideMenuButton {
        id: alertsPageButton
        buttonSource: "qrc:/qt/qml/user/assets/img/alert-black.png"
        buttonPageLink: "qrc:/qt/qml/user/ui/AlertsPage.qml"
        buttonText: "Alerts"
        dataTable: "Alerts"

        anchors {
            left: parent.left
            leftMargin: 10
            top: homePageButton.bottom
            topMargin: 10
        }
    }

    SideMenuButton {
        id: dataPageButton
        buttonSource: "qrc:/qt/qml/user/assets/img/database-black.png"
        buttonPageLink: "qrc:/qt/qml/user/ui/DataPage.qml"
        buttonText: "Data"
        dataTable: "MinifilterLog"

        anchors {
            left: parent.left
            leftMargin: 10
            top: alertsPageButton.bottom
            topMargin: 10
        }
    }
}
