#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "src/core/connectionmanager.h"
#include "src/interface/uinterface.h"
#include "src/managers/ui/uimanager.h"
#include "src/modules/radio/radiocore.h"
#include "src/modules/server/serverconnectioncore.h"
#include "src/network/client.h"

class Core : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(ConnectionStatuses::Status serverConnectionStatus READ serverConnectionStatus WRITE setServerConnectionStatus NOTIFY serverConnectionStatusChanged FINAL)

public:
    explicit Core(QObject *parent = nullptr);

    UIManager* uiManager();

    void registrationSubscribe() override;

    void start();

signals:
    void serverConnectionStatusChanged();

private:
    ConnectionManager m_connectionManager{this};
    UIManager m_uiManager{this};

    RadioCore m_radioCore{this};
    ServerConnectionCore m_serverConnectionCore{this};

    Client m_client;
    QThread m_clientThread{this};

    ConnectionStatuses::Status m_serverConnectionStatus;

    void handleServerConnectionStatus(const QVariantMap& data);

    ConnectionStatuses::Status serverConnectionStatus() const;
    void setServerConnectionStatus(const ConnectionStatuses::Status &newServerConnectionStatus);
};

#endif // CORE_H
