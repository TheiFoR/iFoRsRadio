#include "utablemodel.h"

UTableModel::UTableModel(QObject* parent)
    : QAbstractTableModel(parent) {}

void UTableModel::setColumns(const QStringList& columns) {
    beginResetModel();

    m_columns = columns;

    m_roleNames.clear();
    for (int i = 0; i < m_columns.size(); ++i) {
        m_roleNames[Qt::UserRole + 1 + i] = columns.at(i).toUtf8();
    }

    endResetModel();
}

void UTableModel::update(const quint64 key, const QString& column, const QVariant& value) {
    if (!m_data.contains(key) || !m_columns.contains(column))
        return;

    m_data[key][column] = value;

    int row = m_data.keys().indexOf(key);
    int col = m_columns.indexOf(column);
    if (row >= 0 && col >= 0) {
        emit dataChanged(index(row, col), index(row, col));
    }
}
void UTableModel::update(const quint64 key, const QVariantMap& values) {
    if (!m_data.contains(key)) return;
    auto& data = m_data[key];
    bool changed = false;
    int row = m_data.keys().indexOf(key);

    for (auto it = values.begin(); it != values.end(); ++it) {
        if (m_columns.contains(it.key())) {
            data[it.key()] = it.value();
            changed = true;
        }
    }

    if (changed && row >= 0) {
        int lastCol = m_roleNames.size() - 1;
        emit dataChanged(index(row, 0), index(row, lastCol));
    }
}


void UTableModel::append(const quint64 key, const QString& column, const QVariant& value) {
    if (m_data.contains(key)) return;
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data[key][column] = value;
    endInsertRows();

    setCount(m_data.size());
}

void UTableModel::append(const quint64 key, const QVariantMap &values) {
    if (m_data.contains(key)) return;
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data[key] = values;
    endInsertRows();

    setCount(m_data.size());
}

void UTableModel::remove(const quint64 key) {
    int row = m_data.keys().indexOf(key);
    if (row < 0) return;
    beginRemoveRows(QModelIndex(), row, row);
    m_data.remove(key);
    endRemoveRows();

    setCount(m_data.size());
}

void UTableModel::remove(const QList<quint64>& keys) {
    for (const auto& key : keys) {
        remove(key);
    }
}

void UTableModel::clear() {
    beginResetModel();
    m_data.clear();
    endResetModel();

    setCount(0);
}

bool UTableModel::containce(const quint64 &key) const
{
    return m_data.contains(key);
}

int UTableModel::rowCount(const QModelIndex&) const {
    return m_data.size();
}

int UTableModel::columnCount(const QModelIndex&) const {
    return m_roleNames.size();
}

QVariant UTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return {};

    quint64 key = m_data.keys().at(index.row());
    const auto& row = m_data[key];

    if (role >= Qt::UserRole + 1) {
        QString columnName = QString::fromUtf8(m_roleNames[role]);
        if(!m_columns.contains(columnName)){
            return {};
        }
        return row.value(columnName);
    }

    // fallback на DisplayRole (первый ключ в QVariantMap)
    if (role == Qt::DisplayRole && !row.isEmpty()) {
        return row.begin().value();
    }

    return {};
}

QVariant UTableModel::data(const quint64 &key, const QString &column) const {
    if (!m_data.contains(key) || !m_columns.contains(column))
        return {};
    return m_data[key].value(column);
}

QVariant UTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section >= 0 && section < m_roleNames.size()) {
            return QString::fromUtf8(m_roleNames.values().at(section));
        }
    }
    return {};
}

QHash<int, QByteArray> UTableModel::roleNames() const {
    return m_roleNames;
}

qsizetype UTableModel::count() const {
    return m_count;
}

void UTableModel::setCount(qsizetype newCount) {
    if (m_count == newCount)
        return;
    m_count = newCount;
    emit countChanged(m_count);
}
