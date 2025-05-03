#ifndef CLIENTLISTENER_H
#define CLIENTLISTENER_H

#include <QObject>
#include <QTcpSocket>
#include <QLoggingCategory>

#include "api/external/server.h"
#include "api/external/radio.h"

#include "src/interface/uinterface.h"

class ClientListener : public UInterface
{
    Q_OBJECT
public:
    explicit ClientListener(QObject *parent = nullptr);

    void registrationSubscribe() override;

    void setSocket(QTcpSocket* socket);

private slots:
    void read();

private:
    QTcpSocket* m_socket = nullptr;

    QByteArray m_data;

    QByteArray m_buffer;
    quint64 m_expectedSize = 0;
};

#endif // CLIENTLISTENER_H
