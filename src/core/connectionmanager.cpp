#include "connectionmanager.h"
#include <qdatetime.h>

LOG_DECLARE(ConnectionManager, Core)
LOG_DECLARE(ConnectionManager, Subscribe)
LOG_DECLARE(ConnectionManager, Subscriber)
LOG_DECLARE(ConnectionManager, Command)

ConnectionManager::ConnectionManager(QObject *parent)
    : QObject{parent}
{
    qCInfo(categoryConnectionManagerCore) << "Create";

    connect(this, &ConnectionManager::updateConnections, this, &ConnectionManager::handleUpdateConnections);
}

void ConnectionManager::handleCreateSubscribe(const QString &commandName, UInterface *obj)
{
    qCInfo(categoryConnectionManagerSubscribe) << "Added new command:" << commandName << "from: " << obj;

    m_commandSubscribe[commandName].append(obj);
    emit updateConnections();
}
void ConnectionManager::handleRemoveSubscribe(const QString &commandName, UInterface *obj)
{
    qCInfo(categoryConnectionManagerSubscribe) << "Removed command:" << commandName << "from: " << obj;
}

void ConnectionManager::handleSubscriber(const QString &commandName, UInterface *obj, CallbackPacketFunction function, UInterface::SubscriptionType type)
{
    qCInfo(categoryConnectionManagerSubscriber) << "Added new packet:" << commandName << "from: " << obj;

    m_packetSubscribers[commandName].append(PacketFunctionContext{obj, function, type});
    emit updateConnections();
}
void ConnectionManager::handleUnsubscriber(const QString &commandName, UInterface *obj, CallbackPacketFunction function)
{
    qCInfo(categoryConnectionManagerSubscriber) << "Removed packet:" << commandName << "from: " << obj;
}

void ConnectionManager::handleDone(UInterface *obj)
{
    qCInfo(categoryConnectionManagerCore) << "Object done:" << obj;

    QMetaObject::invokeMethod(obj, "start", Qt::QueuedConnection);

    emit updateConnections();
}

void ConnectionManager::handleSubscriber(const QString &commandName, UInterface *obj, CallbackCommandFunction function, UInterface::SubscriptionType type)
{
    qCInfo(categoryConnectionManagerSubscriber) << "Added new command:" << commandName << "from: " << obj;

    m_commandSubscribers[commandName].append(CommandFunctionContext{obj, function, type});
    emit updateConnections();
}
void ConnectionManager::handleUnsubscriber(const QString &commandName, UInterface *obj, CallbackCommandFunction function)
{
    qCInfo(categoryConnectionManagerSubscriber) << "Removed command:" << commandName << "from: " << obj;
}


void ConnectionManager::handleUpdateConnections()
{
    qCInfo(categoryConnectionManagerCore) << "Update connections";
    for (auto it = m_commandSubscribe.begin(); it != m_commandSubscribe.end(); ++it) {
        const QList<UInterface*>& subscribes = it.value();

        for (UInterface* subscribe : subscribes) {
            connect(subscribe, &UInterface::signalUCommand,
                    this, &ConnectionManager::onCommandReceived,
                    Qt::UniqueConnection);

            connect(subscribe, &UInterface::signalUPacket,
                    this, &ConnectionManager::onPacketReceived,
                    Qt::UniqueConnection);
        }
    }

    const qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    for(auto it = m_lostCommands.begin(); it != m_lostCommands.end();) {
        const LostCommand& lostCommand = *it;
        if (currentTime - lostCommand.lostTime <= 5000) { //5 seconds
            auto itSubscribers = m_commandSubscribers.find(lostCommand.commandName);
            if(itSubscribers != m_commandSubscribers.end()){
                onCommandReceived(lostCommand.commandName, lostCommand.data);
                for (const CommandFunctionContext& ctx : itSubscribers.value()) {
                    auto alreadyExists = std::find_if(
                        m_sentLostCommands.begin(),
                        m_sentLostCommands.end(),
                        [&](const SentLostCommand& s) {
                            return s.obj == ctx.obj && s.commandName == lostCommand.commandName;
                        });

                    if (alreadyExists == m_sentLostCommands.end()) {
                        m_sentLostCommands.emplace_back(SentLostCommand{ctx.obj, lostCommand.commandName});
                    }
                }
            }
            ++it;
        } else {
            it = m_lostCommands.erase(it);
        }
    }
}

void ConnectionManager::onCommandReceived(const QString& commandName, const QVariantMap& data)
{
    qCDebug(categoryConnectionManagerCommand) << "Command received:" << commandName;
    auto sender = this->sender();
    auto it = m_commandSubscribers.find(commandName);
    if (it != m_commandSubscribers.end()) {
        bool onlySelf = true;
        for (const CommandFunctionContext& ctx : it.value()) {
            if (ctx.obj && (ctx.obj != sender || ctx.type == UInterface::SelfHandle)) {
                if(!m_sentLostCommands.empty()){
                    if(m_sentLostCommands.end() != std::find_if(m_sentLostCommands.begin(), m_sentLostCommands.end(), [ctx, commandName](const SentLostCommand& cmd){
                        return cmd.obj == ctx.obj && cmd.commandName == commandName;
                    })){
                        qCDebug(categoryConnectionManagerCommand) << "Skipping lost command for object:" << ctx.obj << "command:" << commandName;
                        continue;
                    }
                }
                qCDebug(categoryConnectionManagerCommand) << "Invoking command for object:" << ctx.obj << "command:" << commandName;
                onlySelf = false;
                QMetaObject::invokeMethod(ctx.obj, [ctx, data]() {
                    ctx.function(data);
                }, Qt::QueuedConnection);
            }
        }
        if(onlySelf){
            m_lostCommands.emplace_back(LostCommand{QDateTime::currentMSecsSinceEpoch(), commandName, data});
            qCWarning(categoryConnectionManagerCommand) << "Lost command detected:" << commandName;
        }
    }
    else{
        m_lostCommands.emplace_back(LostCommand{QDateTime::currentMSecsSinceEpoch(), commandName, data});
        qCWarning(categoryConnectionManagerCommand) << "Lost command detected:" << commandName;
    }
}
void ConnectionManager::onPacketReceived(const QString &commandName, const QVariantMap &data)
{
    auto it = m_packetSubscribers.find(commandName);
    if (it != m_packetSubscribers.end()) {
        for (const PacketFunctionContext& ctx : it.value()) {
            if (ctx.obj) {
                QMetaObject::invokeMethod(ctx.obj, [ctx, commandName, data]() {
                    ctx.function(commandName, data);
                }, Qt::QueuedConnection);
            }
        }
    }
}

