#ifndef USORTFILTERPROXYTABLEMODEL_H
#define USORTFILTERPROXYTABLEMODEL_H

#include <QAbstractProxyModel>
#include <functional>

class USortFilterProxyTableModel : public QAbstractProxyModel
{
    Q_OBJECT

    Q_PROPERTY(bool dynamicSortFilter READ dynamicSortFilter WRITE setDynamicSortFilter NOTIFY dynamicSortFilterChanged FINAL)
public:
    explicit USortFilterProxyTableModel(QObject* parent = nullptr);

    void setSourceModel(QAbstractItemModel* sourceModel) override;

    QModelIndex mapToSource(const QModelIndex& proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex& sourceIndex) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = {}) const override;
    QModelIndex parent(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent = {}) const override;
    int columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // сортировка
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
    void sort(const QString& columnName, Qt::SortOrder order = Qt::AscendingOrder);

    // задать кастомный компаратор по имени колонки
    void setSortFunction(const QString& columnName,
                         std::function<bool(const QVariant&, const QVariant&)> cmp);

    qsizetype count() const;

    bool dynamicSortFilter() const;
    void setDynamicSortFilter(bool newDynamicSortFilter);

signals:
    void countChanged(qsizetype newCount);

    void dynamicSortFilterChanged();

private slots:
    void setCount(qsizetype newCount);

    void onModelReset();
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles);
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);

private:
    qsizetype m_count = 0;
    QVector<int> m_rowOrder; // перестановка строк

    QHash<QString, std::function<bool(const QVariant&, const QVariant&)>> m_sorters;

    int m_lastSortColumn = -1;
    bool m_dynamicSortFilter = true;

    void resort();
};

#endif // USORTFILTERPROXYTABLEMODEL_H
