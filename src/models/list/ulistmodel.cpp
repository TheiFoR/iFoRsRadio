#include "ulistmodel.h"

UListModel::UListModel(QObject* parent)
    : QAbstractListModel(parent)
{}

int UListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

QVariant UListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_data.size())
        return QVariant();

    const QVariantMap& item = m_data.at(index.row());
    QString roleName = m_roles.value(role);
    return item.value(roleName);
}

QHash<int, QByteArray> UListModel::roleNames() const {
    return m_roles;
}

void UListModel::setRoleNames(const QStringList& roles) {
    m_roles.clear();
    int roleId = Qt::UserRole;
    for (const QString& role : roles) {
        m_roles.insert(roleId++, role.toUtf8());
    }
}

void UListModel::appendItem(const QVariantMap& item) {
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append(item);
    endInsertRows();
}

void UListModel::updateItem(int index, const QVariantMap& item) {
    if (index < 0 || index >= m_data.size())
        return;

    m_data[index] = item;
    emit dataChanged(this->index(index), this->index(index));
}

void UListModel::removeItem(int index) {
    if (index < 0 || index >= m_data.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_data.removeAt(index);
    endRemoveRows();
}

void UListModel::clear() {
    beginResetModel();
    m_data.clear();
    endResetModel();
}

QVariantMap UListModel::getItem(int index) const {
    if (index < 0 || index >= m_data.size())
        return QVariantMap();

    return m_data.at(index);
}
