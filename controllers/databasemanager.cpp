#include "databasemanager.h"
#include "MinifilterLogModel.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QVariantMap>
#include <QSqlRecord>
#include <QDateTime>

// =============================================================================== Constructor ===============================================================================

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent} {

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/CephJ/Desktop/CSEC3100 Project/log.db");
    qDebug() << "Opening DB at:" << db.databaseName();

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
        return;
    }

    defineManualRoles();

    // Use the relational model instead of raw query model
    m_model = new MinifilterLogModel(this);
    // runQuery("SELECT * FROM MinifilterLog LIMIT 100");
    m_currentTable = "MinifilterLog";

    // Select all roles by default
    defineManualRoles();
    m_allAvailableColumns = m_manualRoleDefinitions[m_currentTable];
    m_selectedRoles = m_allAvailableColumns;

    // Assume we are ascending sort order
    m_sortOrder = "ASC";
    m_totalPages = 1;

    refreshQuery();
}

// =============================================================================== Exposing to QML ===============================================================================

// Expose the model to UI or other components
QAbstractItemModel* DatabaseManager::model() const {
    return m_model;
}

// Dynamically compute optimal width for a column (for auto-sizing in UI)
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

    return maxWidth;
}

// Dynamically compute optimal width for a column (for auto-sizing in UI)
QString DatabaseManager::convertUnixToDateTime(qint64  unix) {
    return QDateTime::fromSecsSinceEpoch(unix).toString("yyyy-MM-dd hh:mm:ss");
}

// =============================================================================== Query Functions ===============================================================================

// Run a custom SQL query on the model
void DatabaseManager::runQuery(const QString &query) {
    auto logModel = qobject_cast<MinifilterLogModel *>(m_model);
    if (logModel) {
        logModel->refreshQuery(query);
    } else {
        qWarning() << "runQuery called, but current model is not MinifilterLogModel";
    }

    emit queryHasRefreshed();
}

// Refresh the data shown by re-applying filters and sorting
void DatabaseManager::refreshQuery() {
    applyFiltersWithSort(m_filters, m_sortColumn, m_sortOrder);
    updateTotalPages();
}

// =============================================================================== Setting Table ===============================================================================

void DatabaseManager::setCurrentTable(const QString &table) {
    if (m_currentTable != table) {
        m_currentTable = table;

        if (m_manualRoleDefinitions.contains(table)) {
            m_allAvailableColumns = m_manualRoleDefinitions[table];
        } else {
            qWarning() << "No predefined roles for table:" << table;
            m_allAvailableColumns.clear();
        }

        m_selectedRoles = m_allAvailableColumns;
        emit currentTableChanged();
        emit selectedRolesChanged();

        m_filters.clear();
        emit filtersChanged();

        m_currentPage = 0;
        emit currentPageChanged();

        refreshQuery();
    }
}

// Get/set currently selected table name (used in queries)
QString DatabaseManager::currentTable() const {
    return m_currentTable;
}

// =============================================================================== Setting Columns ===============================================================================

void DatabaseManager::defineManualRoles() {
    m_manualRoleDefinitions["MinifilterLog"] = {
        "LogID",
        "SeqNum",
        "OprType",
        "PreOpTime",
        "PostOpTime",
        "ProcessId",
        "ProcessFilePath",
        "ThreadId",
        "MajorOp",
        "MinorOp",
        "IrpFlags",
        "DeviceObj",
        "FileObj",
        "FileTransaction",
        "OpStatus",
        "Information",
        "Arg1",
        "Arg2",
        "Arg3",
        "Arg4",
        "Arg5",
        "Arg6",
        "OpFileName",
        "RequestorMode",
        "RuleID",
        "RuleAction"
    };

    m_manualRoleDefinitions["Alerts"] = {
        "AlertID",
        "Timestamp",
        "AlertMessage"
    };
}

// Return the list of column role names as strings
QStringList DatabaseManager::getRoleNames() const {
    return m_allAvailableColumns;
}

// =============================================================================== Which Columns Are Active ===============================================================================

// Return the currently selected columns
QStringList DatabaseManager::selectedRoles() const {
    return m_selectedRoles;
}

// Replace selected columns
void DatabaseManager::setSelectedRoles(const QStringList &roles) {
    QStringList sorted = sortedByModelOrder(roles);
    if (m_selectedRoles != sorted) {
        m_selectedRoles = sorted;
        emit selectedRolesChanged();
    }
}

QStringList DatabaseManager::sortedByModelOrder(const QStringList &roles) const {
    QStringList ordered;
    for (const QString &col : m_allAvailableColumns) {
        if (roles.contains(col)) {
            ordered.append(col);
        }
    }
    return ordered;
}

// Add/remove a single column to/from selectedRoles
void DatabaseManager::toggleRole(const QString &role, bool selected) {
    // If the role should be selected and it's not already in the list
    if (selected && !m_selectedRoles.contains(role)) {
        // Add it to the list of selected roles
        m_selectedRoles.append(role);

        // If the role should be deselected and it exists in the list
    } else if (!selected && m_selectedRoles.contains(role)) {
        // Remove all instances of this role from the list
        m_selectedRoles.removeAll(role);

        // Also remove the role from active filters if it exists
        if (m_filters.contains(role)) {
            m_filters.remove(role);
            emit filtersChanged(); // Notify that filters have been updated
        }
    }

    // Ensure consistent order
    m_selectedRoles = sortedByModelOrder(m_selectedRoles);

    emit selectedRolesChanged();

    refreshQuery();
}

// =============================================================================== Setting Filtering ===============================================================================

// Get current filter map
QVariantMap DatabaseManager::filters() const {
    return m_filters;
}

// Replace all filters
void DatabaseManager::setFilters(const QVariantMap &filters) {
    if (m_filters != filters) {
        m_filters = filters;
        emit filtersChanged();

        m_currentPage = 0;
        emit currentPageChanged();
    }
}

// Set or update a single filter
void DatabaseManager::setFilter(const QString &key, const QVariant &value) {
    m_filters[key] = value;
    emit filtersChanged();
}

// Apply filters and sort options to build and execute a SQL query
void DatabaseManager::applyFiltersWithSort(const QVariantMap &filters, const QString &sortColumn, const QString &sortOrder) {
    QStringList conditions;

    if (filters.contains("MajorFunction") && !filters["MajorFunction"].toString().isEmpty()) {
        conditions << QString("MajorFunction = '%1'").arg(filters["MajorFunction"].toString());
    }

    if (filters.contains("ProcessName") && !filters["ProcessName"].toString().isEmpty()) {
        conditions << QString("ProcessName LIKE '%1%'").arg(filters["ProcessName"].toString());
    }

    if (filters.contains("FilePath") && !filters["FilePath"].toString().isEmpty()) {
        conditions << QString("FilePath LIKE '%1'").arg(filters["FilePath"].toString());
    }

    QString query = "SELECT " + m_selectedRoles.join(", ") + " FROM " + m_currentTable;

    if (!conditions.isEmpty()) {
        query += " WHERE " + conditions.join(" AND ");
    }

    if (!sortColumn.isEmpty()) {
        query += QString(" ORDER BY %1 %2").arg(sortColumn, sortOrder.toUpper());
    }

    int offset = m_currentPage * m_rowsPerPage;
    query += QString(" LIMIT %1 OFFSET %2").arg(m_rowsPerPage).arg(offset);

    runQuery(query);
}

// =============================================================================== Sorting of Columns ===============================================================================

// Get/set the current column to sort by
QString DatabaseManager::sortColumn() const {
    return m_sortColumn;
}


void DatabaseManager::setSortColumn(const QString &column) {
    if (m_sortColumn != column) {
        m_sortColumn = column;
        emit sortColumnChanged();
    }
}

// Get/set the sort order ("ASC" or "DESC")
QString DatabaseManager::sortOrder() const {
    return m_sortOrder;
}

void DatabaseManager::setSortOrder(const QString &order) {
    if (m_sortOrder != order) {
        m_sortOrder = order;
        emit sortOrderChanged();
    }
}

// =============================================================================== Paging tables ===============================================================================

// Get/set number of rows shown per page
int DatabaseManager::rowsPerPage() const {
    return m_rowsPerPage;
}

void DatabaseManager::setRowsPerPage(int rows) {
    if (m_rowsPerPage != rows) {
        m_rowsPerPage = rows;
        emit rowsPerPageChanged();
        refreshQuery();  // Re-run query with new row limit
    }
}

// Return total page count (currently unused/fixed)
int DatabaseManager::totalPages() const {
    return m_totalPages;
}

int DatabaseManager::currentPage() const {
    return m_currentPage;
}

void DatabaseManager::setCurrentPage(int page) {
    if (page != m_currentPage && page >= 0) {
        m_currentPage = page;
        emit currentPageChanged();
        refreshQuery();  // Trigger query with new offset
    }
}

void DatabaseManager::updateTotalPages() {
    QString countQuery = "SELECT COUNT(*) FROM " + m_currentTable;

    QStringList conditions;
    if (m_filters.contains("MajorFunction") && !m_filters["MajorFunction"].toString().isEmpty())
        conditions << QString("MajorFunction = '%1'").arg(m_filters["MajorFunction"].toString());

    if (m_filters.contains("ProcessName") && !m_filters["ProcessName"].toString().isEmpty())
        conditions << QString("ProcessName LIKE '%%1%%'").arg(m_filters["ProcessName"].toString());

    if (m_filters.contains("FilePath") && !m_filters["FilePath"].toString().isEmpty())
        conditions << QString("FilePath LIKE '%%1%%'").arg(m_filters["FilePath"].toString());

    if (!conditions.isEmpty())
        countQuery += " WHERE " + conditions.join(" AND ");

    QSqlQuery query;
    if (!query.exec(countQuery)) {
        qWarning() << "Count query failed:" << query.lastError().text();
        return;
    }
    int count = 0;
    if (query.next()) {
        count = query.value(0).toInt();
    }

    m_totalPages = (count + m_rowsPerPage - 1) / m_rowsPerPage;
    emit totalPagesChanged();

    // Adjust currentPage if it’s now out of range
    if (m_currentPage >= m_totalPages) {
        m_currentPage = std::max(0, m_totalPages - 1);
        emit currentPageChanged();
    }
}

