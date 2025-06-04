#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtSql>
#include <QQmlContext>

#include <QSqlTableModel>

#include "controllers/DatabaseManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    DatabaseManager dbManager;
    engine.rootContext()->setContextProperty("dbManager", &dbManager);

    // QHash<int, QByteArray> roles = dbManager.model()->roleNames();
    // for (auto it = roles.begin(); it != roles.end(); ++it) {
    //     qDebug() << "Role:" << it.key() << "->" << it.value();
    // }

    engine.loadFromModule("user", "Main");

    return app.exec();
}
