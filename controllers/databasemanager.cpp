#include "databasemanager.h"
#include "MinifilterLogModel.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QVariantMap>
#include <QSqlRecord>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent} {

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/CephJ/Desktop/CSEC3100 Project/TESTING DATABASE/log.db");

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
        return;
    }

    // Use the relational model instead of raw query model
    m_model = new MinifilterLogModel(this);

    // Select all roles by default
    m_selectedRoles = getRoleNames();

    // Assume we are ascending sort order
    m_sortOrder = "ASC";
}

QAbstractItemModel* DatabaseManager::model() const {
    return m_model;
}

void DatabaseManager::runQuery(const QString &query) {
    auto logModel = qobject_cast<MinifilterLogModel *>(m_model);
    if (logModel) {
        logModel->refreshQuery(query);
    } else {
        qWarning() << "runQuery called, but current model is not MinifilterLogModel";
    }
}

int DatabaseManager::computeColumnWidth(int columnIndex, int fontSize) {
    if (!m_model || columnIndex < 0 || columnIndex >= m_model->columnCount())
        return -1;

    QFont font;
    font.setPointSize(fontSize);
    QFontMetrics fm(font);

    int maxWidth = fm.horizontalAdvance(m_model->headerData(columnIndex, Qt::Horizontal).toString()) + 5;

    for (int row = 0; row < m_model->rowCount(); ++row) {
        QString text = m_model->data(m_model->index(row, columnIndex)).toString();
        maxWidth = std::max(maxWidth, fm.horizontalAdvance(text));
    }

    qDebug() << maxWidth;

    return maxWidth;
}

QStringList DatabaseManager::getRoleNames() const {
    QStringList list;
    if (!m_model)
        return list;

    const auto roles = m_model->roleNames();
    for (const auto &name : roles) {
        list << QString::fromUtf8(name);
    }
    return list;
}

// Filter functions

QVariantMap DatabaseManager::filters() const {
    return m_filters;
}

void DatabaseManager::setFilters(const QVariantMap &filters) {
    if (m_filters != filters) {
        m_filters = filters;
        emit filtersChanged();
    }
}

// Sorting funtions

void DatabaseManager::setFilter(const QString &key, const QVariant &value) {
    m_filters[key] = value;
    emit filtersChanged();
}

QString DatabaseManager::sortColumn() const {
    return m_sortColumn;
}

void DatabaseManager::setSortColumn(const QString &column) {
    if (m_sortColumn != column) {
        m_sortColumn = column;
        emit sortColumnChanged();
    }
}

QString DatabaseManager::sortOrder() const {
    return m_sortOrder;
}

void DatabaseManager::setSortOrder(const QString &order) {
    if (m_sortOrder != order) {
        m_sortOrder = order;
        emit sortOrderChanged();
    }
}

// Filter

void DatabaseManager::applyFiltersWithSort(const QVariantMap &filters, const QString &sortColumn, const QString &sortOrder) {
    QStringList conditions;

    if (filters.contains("MajorFunction") && !filters["MajorFunction"].toString().isEmpty()) {
        conditions << QString("MajorFunction = '%1'").arg(filters["MajorFunction"].toString());
    }

    if (filters.contains("ProcessName") && !filters["ProcessName"].toString().isEmpty()) {
        conditions << QString("ProcessName LIKE '%%1%'").arg(filters["ProcessName"].toString());
    }

    if (filters.contains("FilePath") && !filters["FilePath"].toString().isEmpty()) {
        conditions << QString("FilePath LIKE '%%1%'").arg(filters["FilePath"].toString());
    }

    QString query = "SELECT " + m_selectedRoles.join(", ") + " FROM MinifilterLog";

    if (!conditions.isEmpty()) {
        query += " WHERE " + conditions.join(" AND ");
    }

    if (!sortColumn.isEmpty()) {
        query += QString(" ORDER BY %1 %2").arg(sortColumn, sortOrder.toUpper());
    }

    query += " LIMIT 100";

    runQuery(query);
}

// Selection of what attributes to show.

QStringList DatabaseManager::selectedRoles() const {
    return m_selectedRoles;
}

void DatabaseManager::setSelectedRoles(const QStringList &roles) {
    if (m_selectedRoles != roles) {
        m_selectedRoles = roles;
        emit selectedRolesChanged();
    }
}

void DatabaseManager::toggleRole(const QString &role, bool selected) {
    if (selected && !m_selectedRoles.contains(role)) {
        m_selectedRoles.append(role);
        emit selectedRolesChanged();
    } else if (!selected && m_selectedRoles.contains(role)) {
        m_selectedRoles.removeAll(role);
        emit selectedRolesChanged();
    }

    qDebug() << m_selectedRoles;
}

// Paging

int DatabaseManager::rowsPerPage() const {
    return m_rowsPerPage;
}

void DatabaseManager::setRowsPerPage(int rows) {
    if (m_rowsPerPage != rows) {
        m_rowsPerPage = rows;
        emit rowsPerPageChanged();
        refreshQuery();  // Optionally re-run query with new row limit
    }
}

int DatabaseManager::totalPages() const {
    return m_totalPages;
}

// refresh Query

void DatabaseManager::refreshQuery() {
    applyFiltersWithSort(m_filters, m_sortColumn, m_sortOrder);
}
