#include "MinifilterLogModel.h"

MinifilterLogModel::MinifilterLogModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    // Run default query
    refreshQuery("SELECT * FROM MinifilterLog LIMIT 100");
    m_columnRoleNames.clear();
    for (int i = 0; i < columnCount(); ++i)
        m_columnRoleNames << headerData(i, Qt::Horizontal).toString();
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
    for (int i = 0; i < m_columnRoleNames.size(); ++i) {
        roles[Qt::UserRole + i] = m_columnRoleNames[i].toUtf8();
    }
    return roles;
}
