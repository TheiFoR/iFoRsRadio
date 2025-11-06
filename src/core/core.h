#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "src/managers/ui/uimanager.h"
#include "src/core/client/clientinfocore.h"
#include "src/core/radio/radiocore.h"
#include "src/core/server/serverconnectioncore.h"
#include "src/modules/mediaplayer/mediaplayer.h"
#include "src/network/client.h"

#include <SubscriptionManager>

class Core : public SubscriptionNode
{
    Q_OBJECT

    Q_PROPERTY(ConnectionStatuses::Status serverConnectionStatus READ serverConnectionStatus WRITE setServerConnectionStatus NOTIFY serverConnectionStatusChanged FINAL)

public:
    explicit Core(QObject *parent = nullptr);
    ~Core();

    UIManager* uiManager();

    void registrationSubscribe() override;

    void startCore();
    void start() override;

signals:
    void serverConnectionStatusChanged();

private:
    SubscriptionManager m_connectionManager{this};

    RadioCore m_radioCore{this};
    ServerConnectionCore m_serverConnectionCore{this};
    ClientInfoCore m_clientInfoCore{this};
    MediaPlayer m_mediaPlayer{this};

    UIManager m_uiManager{this};

    std::unique_ptr<Client> m_client = nullptr;
    QThread m_clientThread{this};

    ConnectionStatuses::Status m_serverConnectionStatus;

    void handleServerConnectionStatus(const QVariantMap& data);

    ConnectionStatuses::Status serverConnectionStatus() const;
    void setServerConnectionStatus(const ConnectionStatuses::Status &newServerConnectionStatus);
};

#endif // CORE_H
