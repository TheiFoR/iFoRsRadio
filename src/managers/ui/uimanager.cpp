#include "uimanager.h"

LOG_DECLARE(UIManager, Core)

UIManager::UIManager(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryUIManagerCore) << "Create";
}

void UIManager::registrationSubscribe()
{
    qCInfo(categoryUIManagerCore) << "Registration subscription started";

    registrateTransfer(&m_server, this);
    registrateTransfer(&m_client, this);
    registrateTransfer(&m_radioStations, this);

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

