#include "uimanager.h"

UIManager::UIManager(QObject *parent)
    : UInterface{parent}
{}

void UIManager::registrationSubscribe()
{
    emit subscribe(app::model::RadioModel::__name__, this, std::bind(&UIManager::handleUpdateRadioModelPointer, this, std::placeholders::_1));
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

UListModel* UIManager::radioModel()
{
    return m_radioModel;
}

void UIManager::setRadioModel(UListModel* newRadioModel)
{
    if (m_radioModel == newRadioModel)
        return;
    m_radioModel = newRadioModel;
    emit radioModelChanged();
}

void UIManager::handleUpdateRadioModelPointer(const QVariantMap& data)
{
    qWarning() << 2222222;
}
