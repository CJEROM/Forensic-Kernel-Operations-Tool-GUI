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

QString DatabaseManager::convertUnixToDateTime(qint64 filetime) {
    constexpr qint64 TICKS_PER_SECOND = 10000000LL;
    constexpr qint64 UNIX_EPOCH_FILETIME = 116444736000000000LL;

    if (filetime < UNIX_EPOCH_FILETIME || filetime > 200000000000000000LL) {
        return "Invalid timestamp";
    }

    qint64 unixTicks = filetime - UNIX_EPOCH_FILETIME;
    qint64 seconds = unixTicks / TICKS_PER_SECOND;
    qint64 subSecondTicks = unixTicks % TICKS_PER_SECOND;

    QDateTime dt = QDateTime::fromSecsSinceEpoch(seconds, Qt::UTC);

    // Full 100-nanosecond precision (7 decimal places)
    return dt.toString("yyyy-MM-dd hh:mm:ss.") +
           QString("%1").arg(subSecondTicks, 7, 10, QLatin1Char('0'));// + " UTC";
}

qint64 DatabaseManager::convertFlexibleDateTimeToRawTicks(const QString &input) {
    QString value = input.trimmed();
    QString base, fraction = "0000000";

    if (value.contains('.')) {
        QStringList split = value.split('.');
        base = split.value(0);
        fraction = split.value(1).leftJustified(7, '0', true); // pad to 7 digits
    } else {
        base = value;
    }

    // Auto-fill missing components (flexible input like 2025 → 2025-01-01 00:00:00)
    QStringList parts = base.split(' ');
    QString date = parts.value(0);
    QString time = parts.value(1, "00:00:00");

    QStringList dateParts = date.split('-');
    if (dateParts.size() < 3)
        date += QString("-%1").arg("01").repeated(3 - dateParts.size());

    QString fullDateTime = date + " " + time;

    QDateTime dt = QDateTime::fromString(fullDateTime, "yyyy-MM-dd hh:mm:ss");
    dt.setTimeSpec(Qt::UTC); // very important

    if (!dt.isValid()) {
        qWarning() << "Invalid date-time input:" << fullDateTime;
        return -1;
    }

    constexpr qint64 TICKS_PER_SECOND = 10'000'000LL;
    constexpr qint64 FILETIME_EPOCH = 116444736000000000LL;

    bool ok = false;
    qint64 fractionalTicks = fraction.toLongLong(&ok);
    if (!ok) fractionalTicks = 0;

    return FILETIME_EPOCH + (dt.toSecsSinceEpoch() * TICKS_PER_SECOND) + fractionalTicks;
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

// Get unique values of a row in table

QStringList DatabaseManager::getUniqueValues(const QString &columnName) {
    QStringList values;

    if (columnName.isEmpty() || !db.isOpen()) {
        qWarning() << "getUniqueValues: Invalid column or DB not open";
        return values;
    }

    QString queryStr = QString("SELECT DISTINCT %1 FROM %2").arg(columnName, m_currentTable);
    QSqlQuery query(queryStr);

    if (!query.exec()) {
        qWarning() << "Failed to execute unique values query:" << query.lastError().text();
        return values;
    }

    while (query.next()) {
        values << query.value(0).toString();
    }

    values.sort(Qt::CaseInsensitive);  // Optional: sort alphabetically
    return values;
}


// Get


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

void DatabaseManager::removeFilter(const QString &key) {
    if (m_filters.contains(key)) {
        m_filters.remove(key);
        emit filtersChanged();

        m_currentPage = 0;
        emit currentPageChanged();

        refreshQuery();
    } else {
        qDebug() << "Filter not found:" << key;
    }
}


void DatabaseManager::clearFilters() {
    if (!m_filters.isEmpty()) {
        m_filters.clear();
        emit filtersChanged();

        m_currentPage = 0;
        emit currentPageChanged();

        refreshQuery();
    }
}

// Apply filters and sort options to build and execute a SQL query
void DatabaseManager::applyFiltersWithSort(const QVariantMap &filters, const QString &sortColumn, const QString &sortOrder) {
    QStringList conditions;

    // Set of known integer columns
    static const QSet<QString> intColumns = {
        "LogID", "SeqNum", "ProcessId", "ThreadId",
        "RuleID", "MajorOp", "MinorOp", "OpStatus"
    };

    for (auto it = filters.constBegin(); it != filters.constEnd(); ++it) {
        QString column = it.key();
        QString value = it.value().toString().trimmed();

        if (value.isEmpty())
            continue;

        // Choose LIKE or = based on value content or heuristics
        if (value.contains('%') || value.contains('_')) {
            conditions << QString("%1 LIKE '%2'").arg(column, value); // allow wildcard input
        } else if (column.contains("Path", Qt::CaseInsensitive) || column.contains("Name", Qt::CaseInsensitive)) {
            conditions << QString("%1 LIKE '%%1%'").arg(column).arg(value); // partial match for paths/files
        } else if (column == "PreOpTime" || column == "PostOpTime") {
            qint64 raw = convertFlexibleDateTimeToRawTicks(value);
            if (raw != -1) {
                QString op = (column == "PreOpTime") ? ">=" : "<=";
                conditions << QString("%1 %2 %3").arg(column, op).arg(raw);
            }
        } else if (intColumns.contains(column)) {
            bool ok = false;
            int numVal = value.toInt(&ok);
            if (ok) {
                conditions << QString("%1 = %2").arg(column).arg(numVal);
            } else {
                qWarning() << "Invalid integer for column:" << column << "value:" << value;
            }
        } else {
            conditions << QString("%1 = '%2'").arg(column, value);
        }
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

    // Match the same integer logic
    static const QSet<QString> intColumns = {
        "LogID", "SeqNum", "ProcessId", "ThreadId",
        "RuleID", "MajorOp", "MinorOp", "OpStatus"
    };

    for (auto it = m_filters.constBegin(); it != m_filters.constEnd(); ++it) {
        QString column = it.key();
        QString value = it.value().toString().trimmed();

        if (value.isEmpty())
            continue;

        if (value.contains('%') || value.contains('_')) {
            conditions << QString("%1 LIKE '%2'").arg(column, value);
        } else if (column.contains("Path", Qt::CaseInsensitive) || column.contains("Name", Qt::CaseInsensitive)) {
            conditions << QString("%1 LIKE '%%1%'").arg(column).arg(value);
        } else if (column == "PreOpTime" || column == "PostOpTime") {
            qint64 raw = convertFlexibleDateTimeToRawTicks(value);
            if (raw != -1) {
                QString op = (column == "PreOpTime") ? ">=" : "<=";
                conditions << QString("%1 %2 %3").arg(column, op).arg(raw);
            }
        } else if (intColumns.contains(column)) {
            bool ok = false;
            int numVal = value.toInt(&ok);
            if (ok) {
                conditions << QString("%1 = %2").arg(column).arg(numVal);
            } else {
                qWarning() << "Invalid integer for column (in count):" << column << "value:" << value;
            }
        } else {
            conditions << QString("%1 = '%2'").arg(column, value);
        }
    }

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

    // Clamp current page within valid bounds
    if (m_currentPage >= m_totalPages) {
        m_currentPage = std::max(0, m_totalPages - 1);
        emit currentPageChanged();
    }
}


