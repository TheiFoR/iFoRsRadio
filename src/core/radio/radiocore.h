#ifndef RADIOCORE_H
#define RADIOCORE_H

#include <QObject>
#include <QUrl>

#include "src/enums/connectionstatus.h"
#include "src/enums/playstates.h"
#include "src/models/model/utablemodel.h"
#include "src/utils/parameterhandler.h"
#include "src/modules/mediaplayer/trackinfo.h"

#include "api/external/radio.h"
#include "api/internal/server.h"
#include "api/internal/radio.h"
#include "api/internal/mediaPlayer.h"

#include <SubscriptionNode>

class RadioCore : public SubscriptionNode
{
    Q_OBJECT

    Q_PROPERTY(ConnectionStatuses::Status serverConnectionStatus READ serverConnectionStatus WRITE setServerConnectionStatus NOTIFY serverConnectionStatusChanged FINAL)

public:
    explicit RadioCore(QObject *parent = nullptr);
    ~RadioCore();

    void registrationSubscribe() override;

    void start() override;

signals:
    void serverConnectionStatusChanged();

private:
    std::shared_ptr<UTableModel> m_radioModel = nullptr;

    ConnectionStatuses::Status m_serverConnectionStatus;

    void handleServerConnectionStatus(const QVariantMap& data);
    void handleRadioStationList(const QVariantMap& data);

    void handleRadioPlay(const QVariantMap& data);
    void handleRadioStop(const QVariantMap& data);
    void handlePlayerPlaybackStateChanged(const QVariantMap& data);

    ConnectionStatuses::Status serverConnectionStatus() const;
    void setServerConnectionStatus(const ConnectionStatuses::Status &newServerConnectionStatus);
};

#endif // RADIOCORE_H
