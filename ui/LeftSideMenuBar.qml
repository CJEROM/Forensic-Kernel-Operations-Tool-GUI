import QtQuick 2.15

Rectangle {
    id: leftSideMenuBar
    height: parent.height
    width: 180
    color: "#F8F9FA"

    //Will be used to show whether user in admin mode or base mode
    SideMenuButton {
        id: homePageButton
        buttonSource: "qrc:/qt/qml/user/assets/img/home.png"
        buttonPageLink: "qrc:/qt/qml/user/ui/HomePage.qml"
        buttonText: "Home"

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

        anchors {
            left: parent.left
            leftMargin: 10
            top: alertsPageButton.bottom
            topMargin: 10
        }
    }

    // SideMenuButton {
    //     id: rulesPageButton
    //     buttonSource: "qrc:/qt/qml/user/assets/img/rule-black.png"
    //     buttonPageLink: "qrc:/qt/qml/user/ui/RulesPage.qml"
    //     buttonText: "Rules"

    //     anchors {
    //         left: parent.left
    //         leftMargin: 10
    //         top: dataPageButton.bottom
    //         topMargin: 10
    //     }
    // }

    // SideMenuButton {
    //     id: settingsPageButton
    //     buttonSource: "qrc:/qt/qml/user/assets/img/settings-black.png"
    //     buttonPageLink: "qrc:/qt/qml/user/ui/SettingsPage.qml"
    //     buttonText: "Settings"

    //     anchors {
    //         left: parent.left
    //         leftMargin: 10
    //         bottom: parent.bottom
    //         bottomMargin: 10
    //     }
    // }
}
