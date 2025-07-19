#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDebug>
#include <QThread>

#include "api/internal/server.h"
#include "api/external/server.h"

#include "src/enums/connectionstatus.h"
#include "src/interface/uinterface.h"
#include "src/utils/config.h"

class Client : public UInterface
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void registrationSubscribe() override;

public slots:
    void start();

signals:
    void connected();
    void connecting();
    void disconnected();

private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

    void attemptReconnect();

private:
    const quint16 m_reconectInterval = 1000; // 1 seconds

    std::unique_ptr<QTcpSocket> m_socket = nullptr;

    std::unique_ptr<QTimer> m_reconnectTimer = nullptr;

    //QString m_ip = "5.144.98.82";
    QString m_ip = "127.0.0.1";
    quint16 m_port = 1310;

    void saveSettings();
    void loadSettings();
};

#endif // CLIENT_H
