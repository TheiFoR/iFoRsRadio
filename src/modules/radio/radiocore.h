#ifndef RADIOCORE_H
#define RADIOCORE_H

#include <QObject>

#include "src/interface/uinterface.h"
#include "src/models/list/ulistmodel.h"
#include "src/enums/connectionstatus.h"
#include "src/utils/parameterhandler.h"

#include "api/external/radio.h"
#include "api/internal/server.h"

class RadioCore : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(ConnectionStatuses::Status serverConnectionStatus READ serverConnectionStatus WRITE setServerConnectionStatus NOTIFY serverConnectionStatusChanged FINAL)

public:
    explicit RadioCore(QObject *parent = nullptr);

    void registrationSubscribe() override;


signals:
    void serverConnectionStatusChanged();

private:
    UListModel m_radioModel;

    ConnectionStatuses::Status m_serverConnectionStatus;

    void handleServerConnectionStatus(const QVariantMap& data);
    void handleRadioStationList(const QVariantMap& data);

    ConnectionStatuses::Status serverConnectionStatus() const;
    void setServerConnectionStatus(const ConnectionStatuses::Status &newServerConnectionStatus);
};

#endif // RADIOCORE_H
