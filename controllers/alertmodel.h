#ifndef ALERTMODEL_H
#define ALERTMODEL_H

#include <QObject>

class AlertModel : public QObject
{
    Q_OBJECT
public:
    explicit AlertModel(QObject *parent = nullptr);

signals:
};

#endif // ALERTMODEL_H
