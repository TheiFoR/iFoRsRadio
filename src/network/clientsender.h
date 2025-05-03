#ifndef CLIENTSENDER_H
#define CLIENTSENDER_H

#include <QObject>
#include <QLoggingCategory>
#include <QTcpSocket>
#include <QThread>

#include "api/external/server.h"
#include "api/external/radio.h"

#include "src/interface/uinterface.h"
#include "src/types/types.h"

class ClientSender : public UInterface
{
    Q_OBJECT
public:
    explicit ClientSender(QObject *parent = nullptr);

    void registrationSubscribe() override;

    void setSocket(QTcpSocket* socket);

    void sendData(const QString& commandName, const QVariantMap& data);

private:
    QTcpSocket* m_socket = nullptr;
};

#endif // CLIENTSENDER_H
