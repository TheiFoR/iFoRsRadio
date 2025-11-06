#include "subscriptionnode.h"

SubscriptionNode::SubscriptionNode(QObject *parent)
    : QObject{parent}
{}

void SubscriptionNode::registrateTransfer(SubscriptionNode *fromUInterface, SubscriptionNode *toUInterface)
{
    QObject::connect(fromUInterface, QOverload<const QString&, SubscriptionNode*, CallbackCommandFunction, SubscriptionType>::of(&SubscriptionNode::subscribe), toUInterface, QOverload<const QString&, SubscriptionNode*, CallbackCommandFunction, SubscriptionType>::of(&SubscriptionNode::subscribe));
    QObject::connect(fromUInterface, QOverload<const QString&, SubscriptionNode*, CallbackPacketFunction, SubscriptionType>::of(&SubscriptionNode::subscribe), toUInterface, QOverload<const QString&, SubscriptionNode*, CallbackPacketFunction, SubscriptionType>::of(&SubscriptionNode::subscribe));

    QObject::connect(fromUInterface, QOverload<const QString&, SubscriptionNode*, CallbackCommandFunction>::of(&SubscriptionNode::unsubscribe), toUInterface, QOverload<const QString&, SubscriptionNode*, CallbackCommandFunction>::of(&SubscriptionNode::unsubscribe));
    QObject::connect(fromUInterface, QOverload<const QString&, SubscriptionNode*, CallbackPacketFunction>::of(&SubscriptionNode::unsubscribe), toUInterface, QOverload<const QString&, SubscriptionNode*, CallbackPacketFunction>::of(&SubscriptionNode::unsubscribe));

    QObject::connect(fromUInterface, &SubscriptionNode::createSubscribe, toUInterface, &SubscriptionNode::createSubscribe);
    QObject::connect(fromUInterface, &SubscriptionNode::removeSubscribe, toUInterface, &SubscriptionNode::removeSubscribe);

    QObject::connect(fromUInterface, &SubscriptionNode::done, toUInterface, &SubscriptionNode::done);

    fromUInterface->registrationSubscribe();
}

void SubscriptionNode::start()
{

}
