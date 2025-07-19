#include "uiradiostationsmanager.h"

LOG_DECLARE(UIRadioStationsManager, Core)
LOG_DECLARE(UIRadioStationsManager, Model)

UIRadioStationsManager::UIRadioStationsManager(QObject *parent)
    : UInterface{parent}
{}

void UIRadioStationsManager::registrationSubscribe()
{
    qCInfo(categoryUIRadioStationsManagerCore) << "Registration subscription started";

    emit subscribe(app::model::RadioModel::__name__, this, std::bind(&UIRadioStationsManager::handleUpdateRadioModelPointer, this, std::placeholders::_1));

    qCInfo(categoryUIRadioStationsManagerCore) << "Registration subscription completed";
}

void UIRadioStationsManager::handleUpdateRadioModelPointer(const QVariantMap& data)
{
    ParameterHandler ph(data);

    UListModel* radioModelPointer = nullptr;

    if(!ph.handle(radioModelPointer, app::model::RadioModel::Model)){
        qCWarning(categoryUIRadioStationsManagerModel) << "Failed to handle radio model. Data:" << data;
        return;
    }

    m_model = radioModelPointer;

    emit modelChanged();
}

UListModel* UIRadioStationsManager::model()
{
    return m_model;
}
