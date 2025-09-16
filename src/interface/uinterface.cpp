#include "uinterface.h"

UInterface::UInterface(QObject *parent)
    : QObject{parent}
{}

void UInterface::registrateTransfer(UInterface *fromUInterface, UInterface *toUInterface)
{
    QObject::connect(fromUInterface, QOverload<const QString&, UInterface*, CallbackCommandFunction, SubscriptionType>::of(&UInterface::subscribe), toUInterface, QOverload<const QString&, UInterface*, CallbackCommandFunction, SubscriptionType>::of(&UInterface::subscribe));
    QObject::connect(fromUInterface, QOverload<const QString&, UInterface*, CallbackPacketFunction, SubscriptionType>::of(&UInterface::subscribe), toUInterface, QOverload<const QString&, UInterface*, CallbackPacketFunction, SubscriptionType>::of(&UInterface::subscribe));

    QObject::connect(fromUInterface, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::unsubscribe), toUInterface, QOverload<const QString&, UInterface*, CallbackCommandFunction>::of(&UInterface::unsubscribe));
    QObject::connect(fromUInterface, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::unsubscribe), toUInterface, QOverload<const QString&, UInterface*, CallbackPacketFunction>::of(&UInterface::unsubscribe));

    QObject::connect(fromUInterface, &UInterface::createSubscribe, toUInterface, &UInterface::createSubscribe);
    QObject::connect(fromUInterface, &UInterface::removeSubscribe, toUInterface, &UInterface::removeSubscribe);

    QObject::connect(fromUInterface, &UInterface::done, toUInterface, &UInterface::done);

    fromUInterface->registrationSubscribe();
}

void UInterface::start()
{

}
