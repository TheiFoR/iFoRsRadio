#ifndef SERVERCONNECTIONCORE_H
#define SERVERCONNECTIONCORE_H

#include "api/external/server.h"
#include "api/internal/server.h"
#include "api/internal/client.h"

#include "src/interface/uinterface.h"
#include "src/enums/connectionstatus.h"
#include "src/utils/parameterhandler.h"

class ServerConnectionCore : public UInterface
{
public:
    explicit ServerConnectionCore(QObject *parent = nullptr);
    ~ServerConnectionCore();

    void registrationSubscribe() override;

private:
    void handleServerConnectionStatus(const QVariantMap& data);
};

#endif // SERVERCONNECTIONCORE_H
