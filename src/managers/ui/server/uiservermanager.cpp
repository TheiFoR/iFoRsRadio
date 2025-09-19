#include "uiservermanager.h"

LOG_DECLARE(UIServerManager, Core);
LOG_DECLARE(UIServerManager, ServerConnectionStatus);

UIServerManager::UIServerManager(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryUIServerManagerCore) << "Create";
}

UIServerManager::~UIServerManager()
{
    qCInfo(categoryUIServerManagerCore) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categoryUIServerManagerCore) << "Destroy complete";
}

void UIServerManager::registrationSubscribe()
{
    qCInfo(categoryUIServerManagerCore) << "Registration subscription started";

    emit subscribe(app::server::ServerStatus::__name__, this, std::bind(&UIServerManager::handleServerConnectionStatus, this, std::placeholders::_1));

    qCInfo(categoryUIServerManagerCore) << "Registration subscription completed";
}

ConnectionStatuses::Status UIServerManager::connectionStatus() const
{
    return m_connectionStatus;
}

void UIServerManager::setConnectionStatuses(const ConnectionStatuses::Status &newConnectionStatus)
{
    if (m_connectionStatus == newConnectionStatus)
        return;
    m_connectionStatus = newConnectionStatus;
    emit connectionStatusChanged();
}

void UIServerManager::handleServerConnectionStatus(const QVariantMap &data)
{
    ParameterHandler ph(data);

    ConnectionStatuses::Status status = ConnectionStatuses::NoConnection;

    if(!ph.handle(status, app::server::ServerStatus::Status)){
        qCWarning(categoryUIServerManagerServerConnectionStatus) << "Failed to handle connection status. Data:" << data;
        return;
    }

    qCInfo(categoryUIServerManagerServerConnectionStatus) << "New status:" << ConnectionStatuses::toString(status);

    setConnectionStatuses(status);
}
