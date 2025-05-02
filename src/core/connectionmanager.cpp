#include "connectionmanager.h"

ConnectionManager::ConnectionManager(QObject *parent)
    : QObject{parent}
{
    connect(this, &ConnectionManager::updateConnections, this, &ConnectionManager::handleUpdateConnections);
}

void ConnectionManager::handleCreateSubscribe(const QString &commandName, UInterface *obj)
{
    m_commandSubscribe[commandName].append(obj);
    emit updateConnections();
}
void ConnectionManager::handleRemoveSubscribe(const QString &commandName, UInterface *obj)
{

}

void ConnectionManager::handleSubscribe(const QString &commandName, UInterface *obj, CallbackPacketFunction function)
{
    m_packetSubscribers[commandName].append(PacketFunctionContext{obj, function});
    emit updateConnections();
}
void ConnectionManager::handleUnsubscribe(const QString &commandName, UInterface *obj, CallbackPacketFunction function)
{

}

void ConnectionManager::handleUpdateConnections()
{
    for(auto& subscribs: m_commandSubscribe){
        for(auto& subscribe: subscribs){
            for(auto& subscribers: m_commandSubscribers){
                for(auto& subscriber: subscribers){
                    // connect(subscribe, &UInterface::signalUCommand,
                    //         this, [subscriber](const QString& commandName, const QVariantMap& data) {
                    //             // вызываем сохранённую std::function
                    //             subscriber.function(data);
                    //         }, Qt::UniqueConnection | Qt::QueuedConnection);
                }
            }
        }
    }
}

void ConnectionManager::handleSubscribe(const QString &commandName, UInterface *obj, CallbackCommandFunction function)
{
    m_commandSubscribers[commandName].append(CommandFunctionContext{obj, function});
    emit updateConnections();
}
void ConnectionManager::handleUnsubscribe(const QString &commandName, UInterface *obj, CallbackCommandFunction function)
{

}

// void ConnectionManager::handleCommand(const QString &commandName, const QVariantMap &data)
// {
//     // UInterface *senderObject = qobject_cast<UInterface*>(sender());

//     // auto commandSubscribers = m_commandSubscribers.find(commandName);
//     // if(commandSubscribers == m_commandSubscribers.end()){
//     //     return;
//     // }

//     // for(auto subscriber : commandSubscribers.value()){
//     //     if(subscriber.obj == senderObject){
//     //         continue;
//     //     }
//     //     QMetaObject::invokeMethod(
//     //         subscriber.obj,
//     //         [subscriber, data] { subscriber.function(data); },
//     //         Qt::QueuedConnection
//     //         );
//     // }
// }
// void ConnectionManager::handlePacket(const QString &commandName, const QVariantMap &data)
// {
//     // UInterface *senderObject = qobject_cast<UInterface*>(sender());

//     // qWarning() << senderObject << commandName;

//     // auto packetSubscribers = m_packetSubscribers.find(commandName);
//     // if(packetSubscribers == m_packetSubscribers.end()){
//     //     return;
//     // }

//     // for(auto subscriber : packetSubscribers.value()){
//     //     if(subscriber.obj == senderObject){
//     //         continue;
//     //     }
//     //     QMetaObject::invokeMethod(
//     //         subscriber.obj,
//     //         [subscriber, commandName, data] { subscriber.function(commandName, data); },
//     //         Qt::QueuedConnection
//     //     );
//     // }
// }
