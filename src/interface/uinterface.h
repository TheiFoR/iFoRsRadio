#ifndef UINTERFACE_H
#define UINTERFACE_H

#include <QObject>
#include <QVariantMap>
#include <QLoggingCategory>

#include "api/internal/model.h"
#include "src/types/types.h"
#include "src/types/logdef.h"

using namespace std::placeholders;

class UInterface : public QObject
{
    Q_OBJECT
public:
    explicit UInterface(QObject *parent = nullptr);

    enum SubscriptionType {
        Default,
        SelfHandle,
    };

    virtual void registrationSubscribe() = 0;
    void registrateTransfer(UInterface* fromUInterface, UInterface* toUInterface);

    Q_INVOKABLE virtual void start();

signals:
    void signalUCommand(const QString& commandName, const QVariantMap &data = {});
    void signalUPacket(const QString& commandName, const QVariantMap &data = {});

    void createSubscribe(const QString& commandName, UInterface* obj);
    void removeSubscribe(const QString& commandName, UInterface* obj);

    void subscribe(const QString& commandName, UInterface* obj, CallbackCommandFunction function, SubscriptionType type = Default);
    void subscribe(const QString& commandName, UInterface* obj, CallbackPacketFunction function, SubscriptionType type = Default);

    void unsubscribe(const QString& commandName, UInterface* obj, CallbackCommandFunction function);
    void unsubscribe(const QString& commandName, UInterface* obj, CallbackPacketFunction function);

    void done(UInterface* obj);
};

#endif // UINTERFACE_H
