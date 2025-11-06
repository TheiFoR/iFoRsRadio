#ifndef SUBSCRIPTIONSMANAGER_H
#define SUBSCRIPTIONSMANAGER_H

#include <QObject>
#include <queue>

#include <SubscriptionNode>

struct CommandFunctionContext{
    SubscriptionNode* obj;
    CallbackCommandFunction function;
    SubscriptionNode::SubscriptionType type;

    friend QDebug operator<<(QDebug dbg, const CommandFunctionContext& ctx) {
        dbg.nospace() << "CommandFunctionContext(obj=" << ctx.obj << ", type=" << ctx.type << ")";
        return dbg.space();
    }
};

struct PacketFunctionContext{
    SubscriptionNode* obj;
    CallbackPacketFunction function;
    SubscriptionNode::SubscriptionType type;
};

struct LostCommand{
    const qint64 lostTime;
    const QString commandName;
    const QVariantMap data;
};
struct SentLostCommand{
    const SubscriptionNode* obj;
    const QString commandName;
};

class SubscriptionManager : public QObject
{
    Q_OBJECT
public:
    explicit SubscriptionManager(QObject *parent = nullptr);
    ~SubscriptionManager();

public slots:
    void handleCreateSubscribe(const QString& commandName, SubscriptionNode* obj);
    void handleRemoveSubscribe(const QString& commandName, SubscriptionNode* obj);

    void handleSubscriber(const QString& commandName, SubscriptionNode* obj, CallbackCommandFunction function, SubscriptionNode::SubscriptionType type);
    void handleUnsubscriber(const QString& commandName, SubscriptionNode* obj, CallbackCommandFunction function);

    void handleSubscriber(const QString& commandName, SubscriptionNode* obj, CallbackPacketFunction function, SubscriptionNode::SubscriptionType type);
    void handleUnsubscriber(const QString& commandName, SubscriptionNode* obj, CallbackPacketFunction function);

    void handleDone(SubscriptionNode* obj);

signals:
    void updateConnections();

private slots:
    void handleUpdateConnections();

    void onCommandReceived(const QString& commandName, const QVariantMap& data);
    void onPacketReceived(const QString& commandName, const QVariantMap& data);

private:
    QMap<const QString, QList<CommandFunctionContext>> m_commandSubscribers;
    QMap<const QString, QList<SubscriptionNode*>> m_commandSubscribe;

    QMap<const QString, QList<PacketFunctionContext>> m_packetSubscribers;
    QMap<const QString, QList<SubscriptionNode*>> m_packetSubscribe;

    std::list<LostCommand> m_lostCommands;
    std::vector<SentLostCommand> m_sentLostCommands;
};

#endif // SUBSCRIPTIONSMANAGER_H
