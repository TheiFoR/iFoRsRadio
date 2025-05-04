#include "clientsender.h"

LOG_DECLARE(ClientSender, Core)
LOG_DECLARE(ClientSender, Socket)

ClientSender::ClientSender(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryClientSenderCore) << "Create";
}

void ClientSender::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
    connect(socket, &QTcpSocket::errorOccurred, this, &ClientSender::onErrorOccurred);

    qCDebug(categoryClientSenderCore) << "Socket set successfully. Pointer:" << m_socket;
}

void ClientSender::registrationSubscribe()
{
    qCDebug(categoryClientSenderCore) << "Registration subscription started";

    emit subscribe(api::server::ServerConnectionRequest::__name__, this, std::bind(&ClientSender::sendData, this, std::placeholders::_1, std::placeholders::_2));
    emit subscribe(api::radio::RadioStationListRequest::__name__, this, std::bind(&ClientSender::sendData, this, std::placeholders::_1, std::placeholders::_2));

    qCDebug(categoryClientSenderCore) << "Registration subscription completed";
}

void ClientSender::sendData(const QString& commandName, const QVariantMap& data)
{
    if (!m_socket) {
        qCWarning(categoryClientSenderSocket) << "Error: Socket is not set, cannot send data";
        return;
    }
    qCInfo(categoryClientSenderSocket) << "Sending data, command:" << commandName;

    QVariantMap dataPacket;
    QVariantMap sizePacket;

    dataPacket["name"] = commandName;
    dataPacket["data"] = data;

    QByteArray dataBytes;
    QByteArray dataSizeBytes;

    QDataStream dataOut(&dataBytes, QIODevice::WriteOnly);

    dataOut << dataPacket;

    sizePacket["size"] = quint64(dataBytes.size());

    QDataStream sizeDataOut(&dataSizeBytes, QIODevice::WriteOnly);

    sizeDataOut << sizePacket;

    qCDebug(categoryClientSenderSocket) << "Packet datasize size:" << dataSizeBytes.size();
    qCDebug(categoryClientSenderSocket) << "Packet data size:" << dataBytes.size();

    //qCDebug(categoryClientHandlerSocket) << "Raw data to send:" << bytes;
    m_socket->write(dataSizeBytes);
    m_socket->flush();
    m_socket->write(dataBytes);
    m_socket->flush();
}

void ClientSender::onErrorOccurred(QAbstractSocket::SocketError error)
{
    qCWarning(categoryClientSenderSocket) << "Socket error occurred:" << error << m_socket->errorString();
}
