#include "client.h"

LOG_DECLARE(Client, Core)
LOG_DECLARE(Client, Status)
LOG_DECLARE(Client, Connection)
LOG_DECLARE(Client, Settings)

Client::Client(QObject *parent)
: UInterface(parent)
{
    loadSettings();

    qCInfo(categoryClientCore) << "Settings loaded. Starting to connect signals and slots";
}

Client::~Client() {

}

void Client::registrationSubscribe()
{
    qCDebug(categoryClientCore) << "Start registration of subscribers";

    emit createSubscribe(app::server::ServerStatus::__name__, this);

    qCDebug(categoryClientCore) << "Subscriber registration is complete";
}

void Client::start()
{
    qCInfo(categoryClientStatus) << "Initializing client components";

    m_socket = std::make_unique<QTcpSocket>(this);
    m_reconnectTimer = std::make_unique<QTimer>(this);

    m_reconnectTimer->setInterval(m_reconectInterval);
    connect(m_reconnectTimer.get(), &QTimer::timeout, this, &Client::attemptReconnect);

    connect(m_socket.get(), &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket.get(), &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &Client::onErrorOccurred);

    m_reconnectTimer->start();

    attemptReconnect();
}

void Client::attemptReconnect()
{
    // Проверка, не подключён ли уже сокет, либо не в процессе подключения
    if (m_socket->state() == QAbstractSocket::ConnectedState ||
        m_socket->state() == QAbstractSocket::ConnectingState) {
        qCInfo(categoryClientConnection) << "Wait response...";
        return;
    }

    qCInfo(categoryClientConnection) << "Attempting to reconnect to server at"
                                     << m_ip << ":" << m_port;

    m_socket->connectToHost(QHostAddress(m_ip), m_port);
}

void Client::onConnected() {
    qCInfo(categoryClientConnection) << "Connected to server!";

    m_reconnectTimer->stop();
}

void Client::onDisconnected() {
    qCInfo(categoryClientConnection) << "Disconnected from server";

    m_reconnectTimer->start();
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qCWarning(categoryClientConnection) << "Socket error occurred:" << socketError;
}

void Client::saveSettings() {
    Config::setValue("Server", "ip", m_ip);
    Config::setValue("Server", "port", m_port);
    qCInfo(categoryClientSettings) << "Settings saved: ip:" << m_ip << " port:" << m_port;
}

void Client::loadSettings() {
    m_ip = Config::getValue("Server", "ip", m_ip);
    m_port = Config::getValue("Server", "port", m_port);
    qCInfo(categoryClientSettings) << "Settings loaded: ip:" << m_ip << " port:" << m_port;
}
