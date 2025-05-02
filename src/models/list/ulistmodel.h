#ifndef ULISTMODEL_H
#define ULISTMODEL_H

#include <QAbstractListModel>

#include <QAbstractListModel>
#include <QVariant>
#include <QVector>
#include <QHash>

class UListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit UListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void appendItem(const QVariantMap& item);
    Q_INVOKABLE void updateItem(int index, const QVariantMap& item);
    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE void clear();

    Q_INVOKABLE QVariantMap getItem(int index) const;

    void setRoleNames(const QStringList& roles);

private:
    QVector<QVariantMap> m_data;
    QHash<int, QByteArray> m_roles;
};

#endif // ULISTMODEL_H
