#include "clientinfocore.h"

LOG_DECLARE(ClientInfoCore, Core)
LOG_DECLARE(ClientInfoCore, Info)

ClientInfoCore::ClientInfoCore(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryClientInfoCoreCore) << "Create";
}

ClientInfoCore::~ClientInfoCore()
{
    qCInfo(categoryClientInfoCoreCore) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categoryClientInfoCoreCore) << "Destroy complete";
}

void ClientInfoCore::registrationSubscribe()
{
    qCInfo(categoryClientInfoCoreCore) << "Registration subscription started";

    emit createSubscribe(app::client::ClientInfo::__name__, this);
    emit subscribe(api::client::ClientInfo::__name__, this, std::bind(&ClientInfoCore::handleClientInfo, this, std::placeholders::_1));

    qCInfo(categoryClientInfoCoreCore) << "Registration subscription ended";
}

void ClientInfoCore::handleClientInfo(const QVariantMap &data)
{
    ParameterHandler ph(data);

    if(!ph.handle(m_ip, api::client::ClientInfo::Ip)){
        qCWarning(categoryClientInfoCoreInfo) << "Failed to handle ip. Data:" << data;
        return;
    }
    if(!ph.handle(m_port, api::client::ClientInfo::Port)){
        qCWarning(categoryClientInfoCoreInfo) << "Failed to handle port. Data:" << data;
        return;
    }

    m_ipString = QString::number((m_ip & 0xFF000000) >> 24) + "." + QString::number((m_ip & 0x00FF0000) >> 16) + "." + QString::number((m_ip & 0x0000FF00) >> 8) + "." + QString::number(m_ip & 0x000000FF);

    QVariantMap clientInfoMap;
    clientInfoMap[app::client::ClientInfo::Ip] = m_ipString;
    clientInfoMap[app::client::ClientInfo::Port] = m_port;

    emit signalUCommand(app::client::ClientInfo::__name__, clientInfoMap);
}
