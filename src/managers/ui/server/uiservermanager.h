#ifndef UISERVERMANAGER_H
#define UISERVERMANAGER_H

#include "api/internal/server.h"

#include "src/interface/uinterface.h"
#include "src/enums/connectionstatus.h"
#include "src/utils/parameterhandler.h"

class UIServerManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(ConnectionStatuses::Status connectionStatus READ connectionStatus NOTIFY connectionStatusChanged FINAL)

public:
    explicit UIServerManager(QObject *parent = nullptr);
    ~UIServerManager();

    void registrationSubscribe() override;

    ConnectionStatuses::Status connectionStatus() const;

signals:
    void connectionStatusChanged();

private:
    ConnectionStatuses::Status m_connectionStatus = ConnectionStatuses::Status::NoConnection;

    void setConnectionStatuses(const ConnectionStatuses::Status &newConnectionStatus);

    void handleServerConnectionStatus(const QVariantMap& data);
};

#endif // UISERVERMANAGER_H
