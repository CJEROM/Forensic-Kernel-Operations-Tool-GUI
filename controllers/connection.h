#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static bool createConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/CephJ/Desktop/CSEC3100 Project/TESTING DATABASE/log.db");
    //C:/Users/Public/log.db
    //
    qDebug() << "Connecting to SQLite...";
    bool ok = db.open();
    qDebug() << "Open status:" << ok << ", error:" << db.lastError().text();
    if (!ok) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection.\n"
                                          "This example needs SQLite support. Please read "
                                          ), QMessageBox::Cancel);
        return false;
    }

    return true;
}


#endif // CONNECTION_H
