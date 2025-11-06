#ifndef SUBSCRIPTIONNODE_H
#define SUBSCRIPTIONNODE_H

#include <QObject>
#include <QVariantMap>
#include <QLoggingCategory>

#include "api/internal/model.h"
#include "src/types/types.h"
#include "src/types/logdef.h"

using namespace std::placeholders;

class SubscriptionNode : public QObject
{
    Q_OBJECT
public:
    explicit SubscriptionNode(QObject *parent = nullptr);

    enum SubscriptionType {
        Default,
        SelfHandle,
    };

    virtual void registrationSubscribe() = 0;
    void registrateTransfer(SubscriptionNode* fromUInterface, SubscriptionNode* toUInterface);

    Q_INVOKABLE virtual void start();

signals:
    void signalUCommand(const QString& commandName, const QVariantMap &data = {});
    void signalUPacket(const QString& commandName, const QVariantMap &data = {});

    void createSubscribe(const QString& commandName, SubscriptionNode* obj);
    void removeSubscribe(const QString& commandName, SubscriptionNode* obj);

    void subscribe(const QString& commandName, SubscriptionNode* obj, CallbackCommandFunction function, SubscriptionType type = Default);
    void subscribe(const QString& commandName, SubscriptionNode* obj, CallbackPacketFunction function, SubscriptionType type = Default);

    void unsubscribe(const QString& commandName, SubscriptionNode* obj, CallbackCommandFunction function);
    void unsubscribe(const QString& commandName, SubscriptionNode* obj, CallbackPacketFunction function);

    void done(SubscriptionNode* obj);
};

#endif // SUBSCRIPTIONNODE_H
