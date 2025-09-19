#include "uiclientmanager.h"

LOG_DECLARE(UIClientManager, Core);
LOG_DECLARE(UIClientManager, Info);

UIClientManager::UIClientManager(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryUIClientManagerCore) << "Create";
}

UIClientManager::~UIClientManager()
{
    qCInfo(categoryUIClientManagerCore) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categoryUIClientManagerCore) << "Destroy complete";
}

void UIClientManager::registrationSubscribe()
{
    qCInfo(categoryUIClientManagerCore) << "Registration subscription started";

    emit subscribe(app::client::ClientInfo::__name__, this, std::bind(&UIClientManager::handleClientInfo, this, std::placeholders::_1));

    qCInfo(categoryUIClientManagerCore) << "Registration subscription completed";
}


void UIClientManager::handleClientInfo(const QVariantMap &data)
{
    ParameterHandler ph(data);

    QString ip;
    quint16 port;

    if(!ph.handle(ip, app::client::ClientInfo::Ip)){
        qCWarning(categoryUIClientManagerInfo) << "Failed to handle ip. Data" << data;
        return;
    }
    if(!ph.handle(port, app::client::ClientInfo::Port)){
        qCWarning(categoryUIClientManagerInfo) << "Failed to handle port. Data" << data;
        return;
    }

    setAddress(ip + ":" + QString::number(port));
}


QString UIClientManager::address() const
{
    return m_address;
}
void UIClientManager::setAddress(const QString &newAddress)
{
    if (m_address == newAddress)
        return;
    m_address = newAddress;
    emit addressChanged();

    qCDebug(categoryUIClientManagerInfo) << "Set new address:" << m_address;
}
