#include "serverconnectioncore.h"

LOG_DECLARE(ServerConnectionCore, Core)
LOG_DECLARE(ServerConnectionCore, Status)

ServerConnectionCore::ServerConnectionCore(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryServerConnectionCoreCore) << "Create";
}

ServerConnectionCore::~ServerConnectionCore()
{
    qCInfo(categoryServerConnectionCoreCore) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categoryServerConnectionCoreCore) << "Destroy complete";
}

void ServerConnectionCore::registrationSubscribe()
{
    qCInfo(categoryServerConnectionCoreCore) << "Registration subscription started";

    emit createSubscribe(app::server::ServerStatus::__name__, this);
    emit subscribe(api::server::ServerConnectionResponse::__name__, this, std::bind(&ServerConnectionCore::handleServerConnectionStatus, this, std::placeholders::_1));

    qCInfo(categoryServerConnectionCoreCore) << "Registration subscription ended";
}

void ServerConnectionCore::handleServerConnectionStatus(const QVariantMap &data)
{
    ParameterHandler ph(data);

    bool approved = false;

    QVariantMap statusData;
    statusData[app::server::ServerStatus::Status] = ConnectionStatuses::NoConnection;

    ph.handle<ParameterHandler::Optional>(approved, api::server::ServerConnectionResponse::Confirmation);

    if(approved){
        statusData[app::server::ServerStatus::Status] = ConnectionStatuses::Connected;
        qCInfo(categoryServerConnectionCoreStatus) << "Approved!";
    }
    else{
        qCCritical(categoryServerConnectionCoreStatus) << "Connection approved failed!!!";
    }

    emit signalUCommand(app::server::ServerStatus::__name__, statusData);
}
