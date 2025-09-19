#include "core.h"

LOG_DECLARE(Core, Base)
LOG_DECLARE(Core, ServerStatus)
LOG_DECLARE(Core, Module)

Core::Core(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryCoreBase) << "Create";

    m_client = std::make_unique<Client>();
}

Core::~Core()
{
    qCInfo(categoryCoreBase) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);

    if (m_client) {
        // Остановить работу клиента в его потоке и дождаться завершения (blocking)
        QMetaObject::invokeMethod(m_client.get(), "stop", Qt::BlockingQueuedConnection);
    }

    // Остановить поток и дождаться
    m_clientThread.quit();
    if (!m_clientThread.wait(3000)) {
        qCWarning(categoryCoreBase) << "Client thread didn't stop in time, terminating.";
        m_clientThread.terminate();
        m_clientThread.wait();
    }

    // Теперь поток гарантированно остановлен — безопасно удалить объект в текущем потоке
    qCInfo(categoryCoreBase) << "Destroy complete";
}

void Core::registrationSubscribe()
{
    qCInfo(categoryCoreBase) << "Registration subscription started";

    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackCommandFunction, SubscriptionType>::of(&UInterface::subscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackCommandFunction, SubscriptionType>::of(&ConnectionManager::handleSubscriber));
    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackPacketFunction, SubscriptionType>::of(&UInterface::subscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackPacketFunction, SubscriptionType>::of(&ConnectionManager::handleSubscriber));

    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::unsubscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&ConnectionManager::handleUnsubscriber));
    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::unsubscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&ConnectionManager::handleUnsubscriber));

    QObject::connect(this, &UInterface::createSubscribe, &m_connectionManager, &ConnectionManager::handleCreateSubscribe);
    QObject::connect(this, &UInterface::removeSubscribe, &m_connectionManager, &ConnectionManager::handleRemoveSubscribe);

    QObject::connect(this, &UInterface::done, &m_connectionManager, &ConnectionManager::handleDone);


    registrateTransfer(m_client.get(), this);

    registrateTransfer(&m_mediaPlayer, this);

    registrateTransfer(&m_radioCore, this);
    registrateTransfer(&m_serverConnectionCore, this);
    registrateTransfer(&m_clientInfoCore, this);
    registrateTransfer(&m_uiManager, this);

    emit subscribe(app::server::ServerStatus::__name__, this, std::bind(&Core::handleServerConnectionStatus, this, std::placeholders::_1));

    qCInfo(categoryCoreBase) << "Registration subscription completed";

    emit done(this);
}

void Core::startCore()
{
    qCInfo(categoryCoreBase) << "Start thread";

    registrationSubscribe();
}
void Core::start()
{
    qCInfo(categoryCoreModule) << "Start";

    connect(&m_clientThread, &QThread::started, m_client.get(), &Client::start);

    m_client->moveToThread(&m_clientThread);
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
