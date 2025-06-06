cmake_minimum_required(VERSION 3.16)

project(user VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Charts Sql Core Gui Qml Graphs)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appuser
    main.cpp
)

qt_add_qml_module(appuser
    URI user
    VERSION 1.0
    QML_FILES
        Main.qml
        ui/AlertsPage.qml
        ui/DataPage.qml
        ui/HomePage.qml
        ui/LeftSideMenuBar.qml
        ui/PageTitle.qml
        ui/RulesPage.qml
        ui/SettingsPage.qml
        ui/SideMenuButton.qml
        ui/OperationStats.qml
        ui/OperationFileStats.qml
        ui/OperationProcessStats.qml
    RESOURCES
        assets/img/alert-black.png
        assets/img/database-black.png
        assets/img/download-black.png
        assets/img/error-black.png
        assets/img/home.png
        assets/img/remove-black.png
        assets/img/rule-black.png
        assets/img/settings-black.png
        assets/img/sort-black.png
    SOURCES

        controllers/minifilterlogmodel.h
        controllers/minifilterlogmodel.cpp
        controllers/databasemanager.h
        controllers/databasemanager.cpp
        SOURCES controllers/datamodelregistry.h controllers/datamodelregistry.cpp

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appuser PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuser
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appuser PUBLIC
    Qt::Quick
    Qt::Charts
    Qt::Sql
    Qt::Qml
    Qt::Core
    Qt::Gui
    Qt::Graphs
)

include(GNUInstallDirs)
install(TARGETS appuser
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
