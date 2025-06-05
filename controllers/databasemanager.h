#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QQmlEngine>

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>

class DatabaseManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel * model READ model CONSTANT)

    Q_PROPERTY(QString currentTable READ currentTable WRITE setCurrentTable NOTIFY currentTableChanged)

    Q_PROPERTY(QStringList selectedRoles READ selectedRoles WRITE setSelectedRoles NOTIFY selectedRolesChanged)
    Q_PROPERTY(QVariantMap filters READ filters WRITE setFilters NOTIFY filtersChanged)
    Q_PROPERTY(QString sortColumn READ sortColumn WRITE setSortColumn NOTIFY sortColumnChanged)
    Q_PROPERTY(QString sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)

    Q_PROPERTY(int rowsPerPage READ rowsPerPage WRITE setRowsPerPage NOTIFY rowsPerPageChanged)
    Q_PROPERTY(int totalPages READ totalPages NOTIFY totalPagesChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    //QML_ELEMENT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    Q_INVOKABLE int setDatabasePath(const QString &path);
    QAbstractItemModel * model() const;

    QString currentTable() const;
    void setCurrentTable(const QString &table);

    // Functions for table management
    Q_INVOKABLE void runQuery(const QString &query);
    Q_INVOKABLE int computeColumnWidth(int columnIndex, int fontSize = 12);
    Q_INVOKABLE QString convertUnixToDateTime(qint64 timestamp);
    Q_INVOKABLE qint64 convertFlexibleDateTimeToRawTicks(const QString &input);

    Q_INVOKABLE QStringList getRoleNames() const;
    void defineManualRoles();

    Q_INVOKABLE QStringList getUniqueValues(const QString &columnName);

    // Setting what columns to display
    QStringList selectedRoles() const;
    void setSelectedRoles(const QStringList &roles);
    Q_INVOKABLE void toggleRole(const QString &role, bool selected);

    // Filtering Functions
    QVariantMap filters() const;
    Q_INVOKABLE void setFilters(const QVariantMap &filters);
    Q_INVOKABLE void setFilter(const QString &key, const QVariant &value);
    Q_INVOKABLE void applyFiltersWithSort(const QVariantMap &filters, const QString &sortColumn = "", const QString &sortOrder = "ASC");
    Q_INVOKABLE void clearFilters();
    Q_INVOKABLE void removeFilter(const QString &key);

    // Sorting functions
    QString sortColumn() const;
    Q_INVOKABLE void setSortColumn(const QString &column);
    QString sortOrder() const;
    Q_INVOKABLE void setSortOrder(const QString &order);

    //refresh
    Q_INVOKABLE void refreshQuery();

    //for paging information
    int rowsPerPage() const;
    void setRowsPerPage(int rows);
    int totalPages() const;
    int currentPage() const;
    void setCurrentPage(int page);
    void updateTotalPages();

    QStringList sortedByModelOrder(const QStringList &roles) const;

private:
    QSqlDatabase db;
    QAbstractItemModel  *m_model;

    QString m_currentTable;

    QStringList m_allAvailableColumns;

    QMap<QString, QStringList> m_manualRoleDefinitions;

    QStringList m_selectedRoles;
    QVariantMap m_filters;
    QString m_sortColumn;
    QString m_sortOrder = "ASC";

    int m_rowsPerPage = 100;
    int m_totalPages = 1;
    int m_currentPage = 0;

signals:
    void currentTableChanged();

    void selectedRolesChanged();
    void filtersChanged();
    void sortColumnChanged();
    void sortOrderChanged();

    void rowsPerPageChanged();
    void totalPagesChanged();
    void currentPageChanged();

    void queryHasRefreshed();
};

#endif // DATABASEMANAGER_H
