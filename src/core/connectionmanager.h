#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <queue>
#include "src/interface/uinterface.h"

struct CommandFunctionContext{
    UInterface* obj;
    CallbackCommandFunction function;
    UInterface::SubscriptionType type;

    friend QDebug operator<<(QDebug dbg, const CommandFunctionContext& ctx) {
        dbg.nospace() << "CommandFunctionContext(obj=" << ctx.obj << ", type=" << ctx.type << ")";
        return dbg.space();
    }
};

struct PacketFunctionContext{
    UInterface* obj;
    CallbackPacketFunction function;
    UInterface::SubscriptionType type;
};

struct LostCommand{
    const qint64 lostTime;
    const QString commandName;
    const QVariantMap data;
};
struct SentLostCommand{
    const UInterface* obj;
    const QString commandName;
};

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(QObject *parent = nullptr);
    ~ConnectionManager();

public slots:
    void handleCreateSubscribe(const QString& commandName, UInterface* obj);
    void handleRemoveSubscribe(const QString& commandName, UInterface* obj);

    void handleSubscriber(const QString& commandName, UInterface* obj, CallbackCommandFunction function, UInterface::SubscriptionType type);
    void handleUnsubscriber(const QString& commandName, UInterface* obj, CallbackCommandFunction function);

    void handleSubscriber(const QString& commandName, UInterface* obj, CallbackPacketFunction function, UInterface::SubscriptionType type);
    void handleUnsubscriber(const QString& commandName, UInterface* obj, CallbackPacketFunction function);

    void handleDone(UInterface* obj);

signals:
    void updateConnections();

private slots:
    void handleUpdateConnections();

    void onCommandReceived(const QString& commandName, const QVariantMap& data);
    void onPacketReceived(const QString& commandName, const QVariantMap& data);

private:
    QMap<const QString, QList<CommandFunctionContext>> m_commandSubscribers;
    QMap<const QString, QList<UInterface*>> m_commandSubscribe;

    QMap<const QString, QList<PacketFunctionContext>> m_packetSubscribers;
    QMap<const QString, QList<UInterface*>> m_packetSubscribe;

    std::list<LostCommand> m_lostCommands;
    std::vector<SentLostCommand> m_sentLostCommands;
};

#endif // CONNECTIONMANAGER_H
