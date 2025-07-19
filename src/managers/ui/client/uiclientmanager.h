#ifndef UICLIENTMANAGER_H
#define UICLIENTMANAGER_H

#include "api/internal/client.h"

#include "src/interface/uinterface.h"
#include "src/utils/parameterhandler.h"

class UIClientManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)
public:
    explicit UIClientManager(QObject *parent = nullptr);

    void registrationSubscribe() override;

    QString address() const;
    void setAddress(const QString &newAddress);

signals:
    void addressChanged();

private:
    QString m_address = "Unknown";

    void handleClientInfo(const QVariantMap& data);
};

#endif // UICLIENTMANAGER_H
