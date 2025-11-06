#ifndef CLIENTINFOCORE_H
#define CLIENTINFOCORE_H

#include "api/external/client.h"
#include "api/internal/client.h"

#include "src/utils/parameterhandler.h"

#include <SubscriptionNode>

class ClientInfoCore : public SubscriptionNode
{
    Q_OBJECT
public:
    explicit ClientInfoCore(QObject *parent = nullptr);
    ~ClientInfoCore();

    void registrationSubscribe() override;

private:
    QString m_ipString = "";
    quint32 m_ip = 0;
    quint16 m_port = 0;

    void handleClientInfo(const QVariantMap& data);
};

#endif // CLIENTINFOCORE_H
