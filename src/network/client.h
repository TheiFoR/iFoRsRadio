#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QDebug>
#include <QThread>

#include "api/internal/server.h"
#include "api/internal/client.h"
#include "api/external/server.h"
#include "api/external/radio.h"

#include "src/enums/connectionstatus.h"
#include "src/utils/config.h"
#include "src/utils/parameterhandler.h"

#include <SubscriptionNode>

class Client : public SubscriptionNode
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();

    void registrationSubscribe() override;

    Q_INVOKABLE void stop();

public slots:
    void start();

signals:
    void connected();
    void connecting();
    void disconnected();

    void onReceive(const QString& commandName, const QVariantMap& data);

private slots:
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

    void onReadyRead();

    void send(const QString& commandName, const QVariantMap& data = {});

    void attemptReconnect();
    void attempConnectionConfirmation();

private:
    const quint16 m_reconectInterval = 1000; // 1 seconds
    const quint16 m_connectionConfirmationInterval = 5000; // 1 seconds
    QByteArray m_buffer;

    std::unique_ptr<QTcpSocket> m_socket = nullptr;
    std::unique_ptr<QTimer> m_reconnectTimer = nullptr;
    std::unique_ptr<QTimer> m_reconnectionConfirmationTimer = nullptr;

    //QString m_ip = "5.144.98.82";
    QString m_ip = "127.0.0.1";
    quint16 m_port = 1310;

    void parseData();

    void saveSettings();
    void loadSettings();

    void handleServerConnectionStatus(const QVariantMap& data);
};

#endif // CLIENT_H
