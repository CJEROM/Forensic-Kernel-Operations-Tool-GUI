#pragma once

#include <QSqlQueryModel>

class MinifilterLogModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit MinifilterLogModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QVariantMap get(int row) const;

    Q_INVOKABLE void refreshQuery(const QString& query);
};
