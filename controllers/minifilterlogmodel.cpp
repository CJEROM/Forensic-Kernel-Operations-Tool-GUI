#include "MinifilterLogModel.h"

MinifilterLogModel::MinifilterLogModel(QObject *parent)
    : QSqlQueryModel(parent)
{

}

void MinifilterLogModel::refreshQuery(const QString& query) {
    this->setQuery(query);
}

QVariant MinifilterLogModel::data(const QModelIndex &index, int role) const {
    if (role >= Qt::UserRole) {
        int column = role - Qt::UserRole;
        if (column >= 0 && column < columnCount())
            return QSqlQueryModel::data(this->index(index.row(), column), Qt::DisplayRole);
    }
    return QSqlQueryModel::data(index, role);
}

QHash<int, QByteArray> MinifilterLogModel::roleNames() const {
    QHash<int, QByteArray> roles;
    for (int i = 0; i < columnCount(); ++i) {
        roles[Qt::UserRole + i] = headerData(i, Qt::Horizontal).toString().toUtf8();
    }
    return roles;
}

QVariantMap MinifilterLogModel::get(int row) const {
    QVariantMap result;
    if (row < 0 || row >= rowCount()) return result;

    for (int col = 0; col < columnCount(); ++col) {
        QString key = headerData(col, Qt::Horizontal).toString();
        result[key] = data(index(row, col), Qt::DisplayRole);
    }
    return result;
}
