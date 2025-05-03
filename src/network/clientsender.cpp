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

    QVariantMap packet;
    packet["name"] = commandName;
    packet["data"] = data;

    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::WriteOnly);
    out << packet;

    qCDebug(categoryClientSenderSocket) << "Sending data with command:" << commandName;
    // qCDebug(categoryClientSenderSocket) << "Sending data raw:" << bytes;
    // qCDebug(categoryClientSenderSocket) << "Sending data map:" << packet;

    m_socket->write(bytes);
    if (m_socket->waitForBytesWritten()) {
        qCDebug(categoryClientSenderSocket) << "Data sent successfully";
    } else {
        qCWarning(categoryClientSenderSocket) << "Failed to send data";
    }

    m_socket->flush();
}
