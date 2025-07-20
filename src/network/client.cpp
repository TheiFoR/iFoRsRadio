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

    emit createSubscribe(app::server::ServerStatus::__name__, this);
    emit createSubscribe(api::server::ServerConnectionResponse::__name__, this);

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

    qCInfo(categoryClientRead) << "Expected packet size:" << expectedSize << "| Available data size:" << m_buffer.size() - sizeof(quint64);

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

    send(api::server::ServerConnectionRequest::__name__, {});
}

void Client::onDisconnected() {
    qCInfo(categoryClientConnection) << "Disconnected from server";

    m_reconnectTimer->start();
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qCWarning(categoryClientConnection) << "Socket error occurred:" << socketError;
}

void Client::onReadyRead()
{
    qCInfo(categoryClientRead) << "New packet!";
    if (!m_socket) {
        qCWarning(categoryClientRead) << "Error: Socket is not set, cannot read data";
        return;
    }

    while(m_socket->bytesAvailable() > 0){
        m_buffer.append(m_socket->readAll());



        // QVariantMap packet;

        // if(m_expectedSize == 0){
        //     QVariantMap sizePacket;

        //     QByteArray bytes = m_socket->read(m_datasizePacketSize);
        //     QDataStream in(bytes);
        //     in >> sizePacket;

        //     if(!sizePacket.contains("size")){
        //         qCWarning(categoryClientRead) << "Invalid packet structure. Waiting...";
        //         continue;
        //     }

        //     m_expectedSize = sizePacket["size"].toULongLong();
        //     availableBytes -= m_datasizePacketSize;
        //     qCInfo(categoryClientRead) << "Next packet size:" << m_expectedSize << "|" << availableBytes << " bytes left";
        //     continue;
        // }

        // if(availableBytes > m_expectedSize){
        //     m_buffer.append(m_socket->read(m_expectedSize));
        //     availableBytes -= m_expectedSize;
        // }
        // else if(m_buffer.size() + availableBytes > m_expectedSize){
        //     quint64 size = m_expectedSize - m_buffer.size();
        //     m_buffer.append(m_socket->read(size));
        //     availableBytes -= size;
        // }
        // else{
        //     m_buffer.append(m_socket->read(availableBytes));
        //     availableBytes = 0;
        // }

        // if(m_buffer.size() < m_expectedSize){
        //     qCInfo(categoryClientRead) << "Bytes:" << m_buffer.size() << "/" << m_expectedSize << "|" << m_buffer.size() * 100 / m_expectedSize << "% |" << "Waiting...";
        //     continue;
        // }
        // else if(m_buffer.size() == m_expectedSize){
        //     QDataStream in(m_buffer);
        //     in >> packet;
        //     m_buffer.clear();
        //     m_expectedSize = 0;
        //     qCInfo(categoryClientRead) << "Great full packet received!";
        // }
        // else{
        //     qCCritical(categoryClientRead) << "ERROR ---> :" << m_buffer.size() << "/" << m_expectedSize << "|" << m_buffer.size() * 100 / m_expectedSize << "%";
        //     continue;
        // }



        // if (!packet.contains("name") || !packet.contains("data")) {
        //     qCWarning(categoryClientListenerSocket) << "Invalid packet structure";
        //     continue;
        // }

        // QString commandName = packet["name"].toString();
        // QVariantMap data = packet["data"].toMap();

        // qCDebug(categoryClientListenerSocket) << "Received command:" << commandName;

        // emit signalUCommand(commandName, data);

        // QVariantMap clientInfo;

        // quint32 ip = m_socket->localAddress().toIPv4Address();
        // quint16 port = m_socket->localPort();

        // clientInfo[api::client::ClientInfo::Ip] = ip;
        // clientInfo[api::client::ClientInfo::Port] = port;

        // emit signalUCommand(api::client::ClientInfo::__name__, clientInfo);
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
