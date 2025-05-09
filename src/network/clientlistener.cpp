#include "clientlistener.h"

LOG_DECLARE(ClientListener, Core)
LOG_DECLARE(ClientListener, Socket)

ClientListener::ClientListener(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryClientListenerCore) << "Create";
}

void ClientListener::registrationSubscribe()
{
    qCInfo(categoryClientListenerCore) << "Registration subscription started";

    emit createSubscribe(api::server::ServerConnectionResponse::__name__, this);
    emit createSubscribe(api::radio::RadioStationListResponse::__name__, this);

    qCInfo(categoryClientListenerCore) << "Registration subscription completed";
}

void ClientListener::setSocket(QTcpSocket *socket)
{
    m_socket = socket;

    qCDebug(categoryClientListenerSocket) << "Socket set successfully. Pointer:" << m_socket;

    QObject::connect(m_socket, &QTcpSocket::readyRead, this, &ClientListener::read);
}

void ClientListener::read()
{
    qCInfo(categoryClientListenerSocket) << "New packet!";
    if (!m_socket) {
        qCWarning(categoryClientListenerSocket) << "Error: Socket is not set, cannot read data";
        return;
    }

    quint64 availableBytes = m_socket->bytesAvailable();

    qCInfo(categoryClientListenerSocket) << "Available bytes:" << availableBytes;

    while(availableBytes > 0){
        QVariantMap packet;

        if(m_expectedSize == 0){
            QVariantMap sizePacket;

            QByteArray bytes = m_socket->read(m_datasizePacketSize);
            QDataStream in(bytes);
            in >> sizePacket;

            if(!sizePacket.contains("size")){
                qCWarning(categoryClientListenerSocket) << "Invalid packet structure. Waiting...";
                continue;
            }

            m_expectedSize = sizePacket["size"].toULongLong();
            availableBytes -= m_datasizePacketSize;
            qCInfo(categoryClientListenerSocket) << "Next packet size:" << m_expectedSize << "|" << availableBytes << " bytes left";
            continue;
        }

        if(availableBytes > m_expectedSize){
            m_buffer.append(m_socket->read(m_expectedSize));
            availableBytes -= m_expectedSize;
        }
        else if(m_buffer.size() + availableBytes > m_expectedSize){
            quint64 size = m_expectedSize - m_buffer.size();
            m_buffer.append(m_socket->read(size));
            availableBytes -= size;
        }
        else{
            m_buffer.append(m_socket->read(availableBytes));
            availableBytes = 0;
        }

        if(m_buffer.size() < m_expectedSize){
            qCInfo(categoryClientListenerSocket) << "Bytes:" << m_buffer.size() << "/" << m_expectedSize << "|" << m_buffer.size() * 100 / m_expectedSize << "% |" << "Waiting...";
            continue;
        }
        else if(m_buffer.size() == m_expectedSize){
            QDataStream in(m_buffer);
            in >> packet;
            m_buffer.clear();
            m_expectedSize = 0;
            qCInfo(categoryClientListenerSocket) << "Great full packet received!";
        }
        else{
            qCCritical(categoryClientListenerSocket) << "ERROR ---> :" << m_buffer.size() << "/" << m_expectedSize << "|" << m_buffer.size() * 100 / m_expectedSize << "%";
            continue;
        }



        if (!packet.contains("name") || !packet.contains("data")) {
            qCWarning(categoryClientListenerSocket) << "Invalid packet structure";
            continue;
        }

        QString commandName = packet["name"].toString();
        QVariantMap data = packet["data"].toMap();

        qCDebug(categoryClientListenerSocket) << "Received command:" << commandName;

        emit signalUCommand(commandName, data);

        QVariantMap clientInfo;

        quint32 ip = m_socket->localAddress().toIPv4Address();
        quint16 port = m_socket->localPort();

        clientInfo[api::client::ClientInfo::Ip] = ip;
        clientInfo[api::client::ClientInfo::Port] = port;

        emit signalUCommand(api::client::ClientInfo::__name__, clientInfo);
    }
}
