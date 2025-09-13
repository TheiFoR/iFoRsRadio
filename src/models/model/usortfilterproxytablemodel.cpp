#include "usortfilterproxytablemodel.h"
#include "utablemodel.h"
#include <algorithm>

USortFilterProxyTableModel::USortFilterProxyTableModel(QObject* parent)
    : QAbstractProxyModel(parent) {}

void USortFilterProxyTableModel::setSourceModel(QAbstractItemModel* sourceModel) {
    connect(qobject_cast<UTableModel*>(sourceModel), &UTableModel::countChanged,
            this, &USortFilterProxyTableModel::setCount);

    beginResetModel();
    QAbstractProxyModel::setSourceModel(sourceModel);
    m_rowOrder.resize(sourceModel ? sourceModel->rowCount() : 0);
    std::iota(m_rowOrder.begin(), m_rowOrder.end(), 0);
    endResetModel();

    if (sourceModel) {
        connect(sourceModel, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset);
        connect(sourceModel, &QAbstractItemModel::dataChanged, this, &QAbstractItemModel::dataChanged);
        connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &QAbstractItemModel::rowsInserted);
        connect(sourceModel, &QAbstractItemModel::rowsRemoved, this, &QAbstractItemModel::rowsRemoved);

        connect(sourceModel, &QAbstractItemModel::modelReset, this, &USortFilterProxyTableModel::onModelReset);
        connect(sourceModel, &QAbstractItemModel::dataChanged, this, &USortFilterProxyTableModel::onDataChanged);
        connect(sourceModel, &QAbstractItemModel::rowsInserted, this, &USortFilterProxyTableModel::onRowsInserted);
        connect(sourceModel, &QAbstractItemModel::rowsRemoved, this, &USortFilterProxyTableModel::onRowsRemoved);
    }
}

QModelIndex USortFilterProxyTableModel::mapToSource(const QModelIndex& proxyIndex) const {
    if (!proxyIndex.isValid() || !sourceModel()) return {};
    int sourceRow = m_rowOrder.value(proxyIndex.row(), -1);
    return sourceModel()->index(sourceRow, proxyIndex.column());
}

QModelIndex USortFilterProxyTableModel::mapFromSource(const QModelIndex& sourceIndex) const {
    if (!sourceIndex.isValid() || !sourceModel()) return {};
    int proxyRow = m_rowOrder.indexOf(sourceIndex.row());
    return index(proxyRow, sourceIndex.column());
}

QModelIndex USortFilterProxyTableModel::index(int row, int column, const QModelIndex& parent) const {
    if (parent.isValid()) return {};
    return createIndex(row, column);
}

QModelIndex USortFilterProxyTableModel::parent(const QModelIndex& index) const {
    Q_UNUSED(index);
    return {};
}

int USortFilterProxyTableModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid() || !sourceModel()) return 0;
    return m_rowOrder.size();
}

int USortFilterProxyTableModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid() || !sourceModel()) return 0;
    return sourceModel()->columnCount();
}

QVariant USortFilterProxyTableModel::data(const QModelIndex& index, int role) const {
    return mapToSource(index).data(role);
}

QHash<int, QByteArray> USortFilterProxyTableModel::roleNames() const {
    if (sourceModel()) {
        return sourceModel()->roleNames();
    }
    return {};
}

void USortFilterProxyTableModel::sort(int column, Qt::SortOrder order) {
    if (!sourceModel()) return;
    QString columnName = sourceModel()->headerData(column, Qt::Horizontal).toString();

    auto cmpIt = m_sorters.find(columnName);
    if (cmpIt == m_sorters.end()) return;

    auto cmp = cmpIt.value();
    auto src = sourceModel();

    m_lastSortColumn = column;

    beginResetModel();
    std::sort(m_rowOrder.begin(), m_rowOrder.end(),
              [&](int left, int right) {
                  QVariant l = src->index(left, column).data();
                  QVariant r = src->index(right, column).data();
                  bool res = cmp(l, r);
                  return (order == Qt::AscendingOrder) ? res : !res;
              });
    endResetModel();
}

void USortFilterProxyTableModel::sort(const QString &columnName, Qt::SortOrder order)
{
    if (!sourceModel()) return;

    int columnIndex = -1;
    for (int c = 0; c < sourceModel()->columnCount(); ++c) {
        if (sourceModel()->headerData(c, Qt::Horizontal).toString() == columnName) {
            columnIndex = c;
            break;
        }
    }
    if (columnIndex == -1) return;

    sort(columnIndex, order);
}

void USortFilterProxyTableModel::setSortFunction(const QString& columnName,
                                                 std::function<bool(const QVariant&, const QVariant&)> cmp) {
    m_sorters[columnName] = std::move(cmp);
}

qsizetype USortFilterProxyTableModel::count() const {
    return m_count;
}

void USortFilterProxyTableModel::setCount(qsizetype newCount) {
    if (m_count == newCount)
        return;
    m_count = newCount;
    emit countChanged(m_count);
}

void USortFilterProxyTableModel::onModelReset()
{
    resort();
}

void USortFilterProxyTableModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
{
    resort();
}

void USortFilterProxyTableModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    resort();
}

void USortFilterProxyTableModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    resort();
}

void USortFilterProxyTableModel::resort()
{
    if (m_dynamicSortFilter && m_lastSortColumn != -1) {
        sort(m_lastSortColumn);
    }
}

bool USortFilterProxyTableModel::dynamicSortFilter() const
{
    return m_dynamicSortFilter;
}

void USortFilterProxyTableModel::setDynamicSortFilter(bool newDynamicSortFilter)
{
    if (m_dynamicSortFilter == newDynamicSortFilter)
        return;
    m_dynamicSortFilter = newDynamicSortFilter;
    emit dynamicSortFilterChanged();
}
