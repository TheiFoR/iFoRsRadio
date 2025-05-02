#include "uinterface.h"

UInterface::UInterface(QObject *parent)
    : QObject{parent}
{}

void UInterface::registrateTransfer(UInterface *fromUInterface, UInterface *toUInterface)
{
    QObject::connect(fromUInterface, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::subscribe), toUInterface, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::subscribe));
    QObject::connect(fromUInterface, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::subscribe), toUInterface, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::subscribe));

    QObject::connect(fromUInterface, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::unsubscribe), toUInterface, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::unsubscribe));
    QObject::connect(fromUInterface, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::unsubscribe), toUInterface, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::unsubscribe));

    QObject::connect(fromUInterface, &UInterface::createSubscribe, toUInterface, &UInterface::createSubscribe);
    QObject::connect(fromUInterface, &UInterface::removeSubscribe, toUInterface, &UInterface::removeSubscribe);
}

void UInterface::invoke(CallbackCommandFunction function, const QVariantMap& data)
{
    function(data);
}

void UInterface::invoke(CallbackPacketFunction function, const QString& commandName, const QVariantMap& data)
{
    function(commandName, data);
}

