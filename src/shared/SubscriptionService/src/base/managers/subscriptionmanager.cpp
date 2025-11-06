#include "subscriptionmanager.h"
#include <qdatetime.h>

LOG_DECLARE(SubscriptionManager, Core)
LOG_DECLARE(SubscriptionManager, Subscribe)
LOG_DECLARE(SubscriptionManager, Subscriber)
LOG_DECLARE(SubscriptionManager, Command)

SubscriptionManager::SubscriptionManager(QObject *parent)
    : QObject{parent}
{
    qCInfo(categorySubscriptionManagerCore) << "Create";

    connect(this, &SubscriptionManager::updateConnections, this, &SubscriptionManager::handleUpdateConnections);
}

SubscriptionManager::~SubscriptionManager()
{
    qCInfo(categorySubscriptionManagerCore) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categorySubscriptionManagerCore) << "Destroy complete";
}

void SubscriptionManager::handleCreateSubscribe(const QString &commandName, SubscriptionNode *obj)
{
    qCInfo(categorySubscriptionManagerSubscribe) << "Added new command:" << commandName << "from: " << obj;

    m_commandSubscribe[commandName].append(obj);
    emit updateConnections();
}
void SubscriptionManager::handleRemoveSubscribe(const QString &commandName, SubscriptionNode *obj)
{
    qCInfo(categorySubscriptionManagerSubscribe) << "Removed command:" << commandName << "from: " << obj;
}

void SubscriptionManager::handleSubscriber(const QString &commandName, SubscriptionNode *obj, CallbackPacketFunction function, SubscriptionNode::SubscriptionType type)
{
    qCInfo(categorySubscriptionManagerSubscriber) << "Added new packet:" << commandName << "from: " << obj;

    m_packetSubscribers[commandName].append(PacketFunctionContext{obj, function, type});
    emit updateConnections();
}
void SubscriptionManager::handleUnsubscriber(const QString &commandName, SubscriptionNode *obj, CallbackPacketFunction function)
{
    qCInfo(categorySubscriptionManagerSubscriber) << "Removed packet:" << commandName << "from: " << obj;
}

void SubscriptionManager::handleDone(SubscriptionNode *obj)
{
    qCInfo(categorySubscriptionManagerCore) << "Object done:" << obj;

    QMetaObject::invokeMethod(obj, "start", Qt::QueuedConnection);

    emit updateConnections();
}

void SubscriptionManager::handleSubscriber(const QString &commandName, SubscriptionNode *obj, CallbackCommandFunction function, SubscriptionNode::SubscriptionType type)
{
    qCInfo(categorySubscriptionManagerSubscriber) << "Added new command:" << commandName << "from: " << obj;

    m_commandSubscribers[commandName].append(CommandFunctionContext{obj, function, type});
    emit updateConnections();
}
void SubscriptionManager::handleUnsubscriber(const QString &commandName, SubscriptionNode *obj, CallbackCommandFunction function)
{
    qCInfo(categorySubscriptionManagerSubscriber) << "Removed command:" << commandName << "from: " << obj;
}


void SubscriptionManager::handleUpdateConnections()
{
    qCInfo(categorySubscriptionManagerCore) << "Update connections";
    for (auto it = m_commandSubscribe.begin(); it != m_commandSubscribe.end(); ++it) {
        const QList<SubscriptionNode*>& subscribes = it.value();

        for (SubscriptionNode* subscribe : subscribes) {
            connect(subscribe, &SubscriptionNode::signalUCommand,
                    this, &SubscriptionManager::onCommandReceived,
                    Qt::UniqueConnection);

            connect(subscribe, &SubscriptionNode::signalUPacket,
                    this, &SubscriptionManager::onPacketReceived,
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

void SubscriptionManager::onCommandReceived(const QString& commandName, const QVariantMap& data)
{
    qCDebug(categorySubscriptionManagerCommand) << "Command received:" << commandName;
    auto sender = this->sender();
    auto it = m_commandSubscribers.find(commandName);
    if (it != m_commandSubscribers.end()) {
        bool onlySelf = true;
        for (const CommandFunctionContext& ctx : it.value()) {
            qCDebug(categorySubscriptionManagerCommand) << "Checking command for object:" << ctx.obj << "command:" << commandName << "type:" << ctx.type << "sender:" << sender << "this:" << this;
            if (ctx.obj && (ctx.obj != sender || ctx.type == SubscriptionNode::SelfHandle)) {
                if(!m_sentLostCommands.empty()){
                    if(m_sentLostCommands.end() != std::find_if(m_sentLostCommands.begin(), m_sentLostCommands.end(), [ctx, commandName](const SentLostCommand& cmd){
                        return cmd.obj == ctx.obj && cmd.commandName == commandName;
                    })){
                        qCDebug(categorySubscriptionManagerCommand) << "Skipping lost command for object:" << ctx.obj << "command:" << commandName;
                        continue;
                    }
                }
                qCDebug(categorySubscriptionManagerCommand) << "Invoking command for object:" << ctx.obj << "command:" << commandName;
                onlySelf = false;
                QMetaObject::invokeMethod(ctx.obj, [ctx, data]() {
                    ctx.function(data);
                }, Qt::QueuedConnection);
            }
        }
        if (onlySelf) {
            const auto &subscribers = it.value(); // список подписчиков
            const auto subsIt = m_commandSubscribe.find(commandName); // список эмиттеров для этой команды

            QObject* emmitter = nullptr;
            bool onlySelfCase = false;
            if (subsIt != m_commandSubscribe.end() && subscribers.size() == 1) {
                SubscriptionNode* singleSubscriber = subscribers.first().obj;

                // Проверяем, совпадает ли подписчик хотя бы с одним из эмиттеров
                for (SubscriptionNode* emitter : subsIt.value()) {
                    if (emitter == singleSubscriber) {
                        onlySelfCase = true;
                        break;
                    }
                }
            }

            if (onlySelfCase) {
                m_lostCommands.emplace_back(LostCommand{
                    QDateTime::currentMSecsSinceEpoch(),
                    commandName,
                    data
                });
                qCWarning(categorySubscriptionManagerCommand)
                << "Lost command detected:" << commandName
                << "Only one subscriber, and it matches one of the emitters."
                << "Command is effectively unhandled by any other class.";
            }
        }
    }
    else{
        m_lostCommands.emplace_back(LostCommand{QDateTime::currentMSecsSinceEpoch(), commandName, data});
        qCWarning(categorySubscriptionManagerCommand) << "Lost command detected:" << commandName << "no handlers.";
    }
}
void SubscriptionManager::onPacketReceived(const QString &commandName, const QVariantMap &data)
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

