#include "core.h"

Core::Core(QObject *parent)
    : UInterface{parent}
{
    registrationSubscribe();
}

void Core::registrationSubscribe()
{
    registrateTransfer(&m_uiManager, this);
    registrateTransfer(&m_radioCore, this);

    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::subscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&ConnectionManager::handleSubscribe));
    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::subscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&ConnectionManager::handleSubscribe));

    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::unsubscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&ConnectionManager::handleSubscribe));
    QObject::connect(this, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::unsubscribe), &m_connectionManager, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&ConnectionManager::handleSubscribe));

    QObject::connect(this, &UInterface::createSubscribe, &m_connectionManager, &ConnectionManager::handleCreateSubscribe);
    QObject::connect(this, &UInterface::removeSubscribe, &m_connectionManager, &ConnectionManager::handleRemoveSubscribe);

    m_uiManager.registrationSubscribe();
    m_radioCore.registrationSubscribe();
}

UIManager* Core::uiManager()
{
    return &m_uiManager;
}
