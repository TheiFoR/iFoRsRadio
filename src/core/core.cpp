#include "core.h"

LOG_DECLARE(Core, Base)
LOG_DECLARE(Core, ServerStatus)

Core::Core(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryCoreBase) << "Create";
}

void Core::registrationSubscribe()
{
    qCInfo(categoryCoreBase) << "Registration subscription started";

    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::subscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&ConnectionManager::handleSubscriber));
    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::subscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&ConnectionManager::handleSubscriber));

    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::unsubscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&ConnectionManager::handleSubscriber));
    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::unsubscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&ConnectionManager::handleSubscriber));

    QObject::connect(this, &UInterface::createSubscribe, &m_connectionManager, &ConnectionManager::handleCreateSubscribe);
    QObject::connect(this, &UInterface::removeSubscribe, &m_connectionManager, &ConnectionManager::handleRemoveSubscribe);

    registrateTransfer(&m_uiManager, this);

    registrateTransfer(&m_client, this);

    registrateTransfer(&m_radioCore, this);
    registrateTransfer(&m_serverConnectionCore, this);
    registrateTransfer(&m_clientInfoCore, this);

    emit subscribe(app::server::ServerStatus::__name__, this, std::bind(&Core::handleServerConnectionStatus, this, std::placeholders::_1));

    qCInfo(categoryCoreBase) << "Registration subscription completed";
}

void Core::start()
{
    qCInfo(categoryCoreBase) << "Start";

    registrationSubscribe();

    connect(&m_clientThread, &QThread::started, &m_client, &Client::start);
    m_client.moveToThread(&m_clientThread);
    m_clientThread.start();
}

void Core::handleServerConnectionStatus(const QVariantMap &data)
{
    ParameterHandler ph(data);

    ConnectionStatuses::Status status = ConnectionStatuses::NoConnection;

    if(!ph.handle(status, app::server::ServerStatus::Status)){
        qCWarning(categoryCoreServerStatus) << "Failed to handle connection status. Data:" << data;
        return;
    }

    qCInfo(categoryCoreServerStatus) << "New status:" << ConnectionStatuses::toString(status);

    setServerConnectionStatus(status);
}

UIManager* Core::uiManager()
{
    return &m_uiManager;
}

ConnectionStatuses::Status Core::serverConnectionStatus() const
{
    return m_serverConnectionStatus;
}
void Core::setServerConnectionStatus(const ConnectionStatuses::Status &newServerConnectionStatus)
{
    if (m_serverConnectionStatus == newServerConnectionStatus)
        return;
    m_serverConnectionStatus = newServerConnectionStatus;
    emit serverConnectionStatusChanged();
}
