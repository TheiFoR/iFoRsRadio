#include "uimanager.h"

LOG_DECLARE(UIManager, Core)

UIManager::UIManager(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryUIManagerCore) << "Create";
}

UIManager::~UIManager()
{
    qCInfo(categoryUIManagerCore) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categoryUIManagerCore) << "Destroy complete";
}

void UIManager::registrationSubscribe()
{
    qCInfo(categoryUIManagerCore) << "Registration subscription started";

    registrateTransfer(&m_server, this);
    registrateTransfer(&m_client, this);
    registrateTransfer(&m_radioStations, this);
    registrateTransfer(&m_mediaplayer, this);

    qCInfo(categoryUIManagerCore) << "Registration subscription completed";
}

Pages::Page UIManager::currentPage() const
{
    return m_currentPage;
}
void UIManager::setCurrentPage(const Pages::Page &newCurrentPage)
{
    if (m_currentPage == newCurrentPage)
        return;
    m_currentPage = newCurrentPage;
    emit currentPageChanged();
}

UIServerManager* UIManager::server()
{
    return &m_server;
}
UIClientManager* UIManager::client()
{
    return &m_client;
}
UIRadioStationsManager* UIManager::radioStations()
{
    return &m_radioStations;
}


quint8 UIManager::styleIndex() const
{
    return m_styleIndex;
}

void UIManager::setStyleIndex(quint8 newStyleIndex)
{
    if (m_styleIndex == newStyleIndex)
        return;
    m_styleIndex = newStyleIndex;
    emit styleIndexChanged();
}

UIMediaPlayerManager* UIManager::mediaplayer()
{
    return &m_mediaplayer;
}
