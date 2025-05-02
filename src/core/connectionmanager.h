#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include "src/interface/uinterface.h"

struct CommandFunctionContext{
    UInterface* obj;
    CallbackCommandFunction function;
};

struct PacketFunctionContext{
    UInterface* obj;
    CallbackPacketFunction function;
};

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(QObject *parent = nullptr);

public slots:
    void handleCreateSubscribe(const QString& commandName, UInterface* obj);
    void handleRemoveSubscribe(const QString& commandName, UInterface* obj);

    void handleSubscribe(const QString& commandName, UInterface* obj, CallbackCommandFunction function);
    void handleUnsubscribe(const QString& commandName, UInterface* obj, CallbackCommandFunction function);

    void handleSubscribe(const QString& commandName, UInterface* obj, CallbackPacketFunction function);
    void handleUnsubscribe(const QString& commandName, UInterface* obj, CallbackPacketFunction function);    

signals:
    void updateConnections();

private slots:
    void handleUpdateConnections();

private:
    QMap<const QString, QList<CommandFunctionContext>> m_commandSubscribers;
    QMap<const QString, QList<UInterface*>> m_commandSubscribe;

    QMap<const QString, QList<PacketFunctionContext>> m_packetSubscribers;
    QMap<const QString, QList<UInterface*>> m_packetSubscribe;
};

#endif // CONNECTIONMANAGER_H
