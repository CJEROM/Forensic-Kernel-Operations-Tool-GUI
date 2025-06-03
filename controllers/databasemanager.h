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

    Q_PROPERTY(QStringList selectedRoles READ selectedRoles WRITE setSelectedRoles NOTIFY selectedRolesChanged)
    Q_PROPERTY(QVariantMap filters READ filters WRITE setFilters NOTIFY filtersChanged)
    Q_PROPERTY(QString sortColumn READ sortColumn WRITE setSortColumn NOTIFY sortColumnChanged)
    Q_PROPERTY(QString sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)

    Q_PROPERTY(int rowsPerPage READ rowsPerPage WRITE setRowsPerPage NOTIFY rowsPerPageChanged)
    Q_PROPERTY(int totalPages READ totalPages NOTIFY totalPagesChanged)
    //QML_ELEMENT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    QAbstractItemModel * model() const;

    // Functions for table management
    Q_INVOKABLE void runQuery(const QString &query);
    Q_INVOKABLE int computeColumnWidth(int columnIndex, int fontSize = 12);
    Q_INVOKABLE QStringList getRoleNames() const;

    // Setting what columns to display
    QStringList selectedRoles() const;
    void setSelectedRoles(const QStringList &roles);
    Q_INVOKABLE void toggleRole(const QString &role, bool selected);

    // Filtering Functions
    QVariantMap filters() const;
    void setFilters(const QVariantMap &filters);
    Q_INVOKABLE void setFilter(const QString &key, const QVariant &value);
    Q_INVOKABLE void applyFiltersWithSort(const QVariantMap &filters, const QString &sortColumn = "", const QString &sortOrder = "ASC");

    // Sorting functions
    QString sortColumn() const;
    void setSortColumn(const QString &column);
    QString sortOrder() const;
    void setSortOrder(const QString &order);

    //refresh
    Q_INVOKABLE void refreshQuery();

    //for paging information
    int rowsPerPage() const;
    void setRowsPerPage(int rows);
    int totalPages() const;

private:
    QSqlDatabase db;
    QAbstractItemModel  *m_model;

    QStringList m_selectedRoles;
    QVariantMap m_filters;
    QString m_sortColumn;
    QString m_sortOrder = "ASC";

    int m_rowsPerPage = 100;
    int m_totalPages = 1;

signals:
    void selectedRolesChanged();
    void filtersChanged();
    void sortColumnChanged();
    void sortOrderChanged();

    void rowsPerPageChanged();
    void totalPagesChanged();
};

#endif // DATABASEMANAGER_H
