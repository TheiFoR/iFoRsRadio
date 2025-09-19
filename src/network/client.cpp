#include "client.h"

LOG_DECLARE(Client, Core)
LOG_DECLARE(Client, Status)
LOG_DECLARE(Client, Connection)
LOG_DECLARE(Client, Settings)
LOG_DECLARE(Client, Write)
LOG_DECLARE(Client, Read)

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

    emit createSubscribe(app::client::ClientInfo::__name__, this);
    emit createSubscribe(app::server::ServerStatus::__name__, this);

    emit createSubscribe(api::server::ServerConnectionResponse::__name__, this);

    emit subscribe(api::server::ServerConnectionResponse::__name__, this, std::bind(&Client::handleServerConnectionStatus, this, std::placeholders::_1), UInterface::SubscriptionType::SelfHandle);
    emit subscribe(api::radio::RadioStationListRequest::__name__, this, std::bind(&Client::send, this, std::placeholders::_1, std::placeholders::_2));

    qCDebug(categoryClientCore) << "Subscriber registration is complete";
}

void Client::stop()
{
    qCInfo(categoryClientStatus) << "Stopping client components";

    if (m_socket) {
        if (m_socket->state() == QAbstractSocket::ConnectedState ||
            m_socket->state() == QAbstractSocket::ConnectingState) {
            m_socket->disconnectFromHost();
            if (m_socket->state() != QAbstractSocket::UnconnectedState) {
                m_socket->waitForDisconnected(3000); // Wait up to 3 seconds for disconnection
            }
        }
        m_socket->close();
    }

    if (m_reconnectTimer) {
        m_reconnectTimer->stop();
    }

    if (m_reconnectionConfirmationTimer) {
        m_reconnectionConfirmationTimer->stop();
    }

    saveSettings();

    qCInfo(categoryClientStatus) << "Client components stopped";
}

void Client::start()
{
    qCInfo(categoryClientStatus) << "Initializing client components";

    m_socket = std::make_unique<QTcpSocket>(this);
    m_reconnectTimer = std::make_unique<QTimer>(this);
    m_reconnectionConfirmationTimer = std::make_unique<QTimer>(this);

    m_reconnectTimer->setInterval(m_reconectInterval);
    m_reconnectionConfirmationTimer->setInterval(m_connectionConfirmationInterval);
    connect(m_reconnectTimer.get(), &QTimer::timeout, this, &Client::attemptReconnect);
    connect(m_reconnectionConfirmationTimer.get(), &QTimer::timeout, this, &Client::attempConnectionConfirmation);

    connect(m_socket.get(), &QTcpSocket::connected, this, &Client::onConnected);
    connect(m_socket.get(), &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &Client::onErrorOccurred);

    connect(m_socket.get(), &QTcpSocket::readyRead, this, &Client::onReadyRead);

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

void Client::attempConnectionConfirmation()
{
    send(api::server::ServerConnectionRequest::__name__);
}

void Client::parseData()
{
    if(m_buffer.size() < sizeof(quint64)){
        qCWarning(categoryClientRead) << "Buffer size is less than expected size, waiting for more data...";
        return;
    }

    QDataStream in(m_buffer);

    quint64 expectedSize;
    in >> expectedSize;

    if(expectedSize == 0){
        qCWarning(categoryClientRead) << "Received packet with size 0, waiting for more data...";
        return;
    }
    if(expectedSize > m_buffer.size() - sizeof(quint64)){
        qCWarning(categoryClientRead) << "Expected packet size is larger than available data, waiting for more data...";
        return;
    }

    qCDebug(categoryClientRead) << "Expected packet size:" << expectedSize << "| Available data size:" << m_buffer.size() - sizeof(quint64);

    QVariantMap packet;
    in >> packet;

    m_buffer.remove(0, sizeof(quint64) + expectedSize);
    qCInfo(categoryClientRead) << "Packet received, remaining buffer size:" << m_buffer.size();

    if (!packet.contains("name") || !packet.contains("data")) {
        qCWarning(categoryClientRead) << "Invalid packet structure";
        return;
    }

    QString commandName = packet["name"].toString();
    QVariantMap data = packet["data"].toMap();

    qCInfo(categoryClientRead) << "Received command:" << commandName;

    emit signalUCommand(commandName, data);
}

void Client::onConnected() {
    qCInfo(categoryClientConnection) << "Connected to server!";

    m_reconnectTimer->stop();

    QVariantMap clienInfoData;
    clienInfoData[app::client::ClientInfo::Ip] = m_socket->localAddress().toString();
    clienInfoData[app::client::ClientInfo::Port] = m_socket->localPort();
    emit signalUCommand(app::client::ClientInfo::__name__, clienInfoData);

    m_reconnectionConfirmationTimer->start();

    attempConnectionConfirmation();
}

void Client::onDisconnected() {
    qCInfo(categoryClientConnection) << "Disconnected from server";

    QVariantMap serverStatusData;
    serverStatusData[app::server::ServerStatus::Status] = ConnectionStatuses::Status::NoConnection;
    emit signalUCommand(app::server::ServerStatus::__name__, serverStatusData);

    QVariantMap clienInfoData;
    clienInfoData[app::client::ClientInfo::Ip] = "Unknown";
    clienInfoData[app::client::ClientInfo::Port] = 00000;
    emit signalUCommand(app::client::ClientInfo::__name__, clienInfoData);

    m_reconnectTimer->start();
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qCWarning(categoryClientConnection) << "Socket error occurred:" << socketError;
}

void Client::onReadyRead()
{
    qCDebug(categoryClientRead) << "New packet!";
    if (!m_socket) {
        qCWarning(categoryClientRead) << "Error: Socket is not set, cannot read data";
        return;
    }

    while(m_socket->bytesAvailable() > 0){
        m_buffer.append(m_socket->readAll());
    }

    parseData();
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

void Client::handleServerConnectionStatus(const QVariantMap &data)
{
    ParameterHandler ph(data);

    bool approved = false;

    ph.handle<ParameterHandler::Optional>(approved, api::server::ServerConnectionResponse::Confirmation);

    if(approved){
        m_reconnectionConfirmationTimer->stop();
    }
    else{
        return;
    }
}

void Client::send(const QString &commandName, const QVariantMap &data)
{
    if (!m_socket) {
        qCWarning(categoryClientWrite) << "Error: Socket is not set, cannot send data";
        return;
    }
    qCInfo(categoryClientWrite) << "Sending data, command:" << commandName;

    QVariantMap dataPacket;
    QVariantMap sizePacket;

    dataPacket["name"] = commandName;
    dataPacket["data"] = data;

    QByteArray dataBytes;
    QByteArray dataSizeBytes;

    QDataStream dataOut(&dataBytes, QIODevice::WriteOnly);

    dataOut << dataPacket;

    QDataStream sizeDataOut(&dataSizeBytes, QIODevice::WriteOnly);

    sizeDataOut << quint64(dataBytes.size());

    qCDebug(categoryClientWrite) << "Packet datasize size:" << dataSizeBytes.size();
    qCDebug(categoryClientWrite) << "Packet data size:" << dataBytes.size();

    m_socket->write(dataSizeBytes);
    m_socket->flush();

    m_socket->write(dataBytes);
    m_socket->flush();
}
