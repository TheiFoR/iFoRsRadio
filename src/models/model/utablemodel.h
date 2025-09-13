#ifndef UTABLEMODEL_H
#define UTABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QVariant>
#include <QStringList>

class UTableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(qsizetype count READ count NOTIFY countChanged FINAL)

public:
    explicit UTableModel(QObject* parent = nullptr);

    void setColumns(const QStringList& columns);

    void update(const quint64 key, const QString& column, const QVariant& value);                  // update one field
    void update(const quint64 key, const QVariantMap& values);              // update multiple fields
    void append(const quint64 key, const QString& column, const QVariant& value);              // append one row
    void append(const quint64 key, const QVariantMap& values);              // append one row
    void remove(const quint64 key);                                         // remove one row
    void remove(const QList<quint64>& keys);                                 // remove multiple rows
    void clear();                                                             // remove all rows

    bool containce(const quint64& key) const;

    // QAbstractTableModel overrides
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant data(const quint64& key, const QString& column) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    qsizetype count() const;

signals:
    void countChanged(qsizetype newCount);

private:
    QMap<quint64, QMap<QString, QVariant>> m_data;
    QHash<int, QByteArray> m_roleNames;
    QStringList m_columns;

    qsizetype m_count = 0;

    void setCount(qsizetype newCount);
};

#endif // UTABLEMODEL_H
