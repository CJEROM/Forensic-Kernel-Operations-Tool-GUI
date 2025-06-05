#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtSql>
#include <QQmlContext>

#include <QSqlQueryModel>
#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QQmlEngine>

#include "controllers/DatabaseManager.h"
#include "controllers/DataModelRegistry.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    DatabaseManager dbManager;
    engine.rootContext()->setContextProperty("dbManager", &dbManager);

    DataModelRegistry *dataModelRegistry = new DataModelRegistry;
    engine.rootContext()->setContextProperty("dataModelRegistry", dataModelRegistry);

    engine.loadFromModule("user", "Main");

    return app.exec();
}
