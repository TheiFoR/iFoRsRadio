#include "uiradiostationsmanager.h"

LOG_DECLARE(UIRadioStationsManager, Core)
LOG_DECLARE(UIRadioStationsManager, Model)

UIRadioStationsManager::UIRadioStationsManager(QObject *parent)
    : UInterface{parent}
{
}

void UIRadioStationsManager::registrationSubscribe()
{
    qCInfo(categoryUIRadioStationsManagerCore) << "Registration subscription started";

    emit createSubscribe(app::radio::Play::__name__, this);
    emit createSubscribe(app::radio::Stop::__name__, this);

    emit subscribe(app::model::RadioModel::__name__, this, std::bind(&UIRadioStationsManager::handleUpdateRadioModelPointer, this, std::placeholders::_1));

    qCInfo(categoryUIRadioStationsManagerCore) << "Registration subscription completed";
}

void UIRadioStationsManager::play(quint64 radioStationId)
{
    if(!m_model){
        qCWarning(categoryUIRadioStationsManagerModel) << "Radio model is not set";
        return;
    }

    if(!m_model->containce(radioStationId)){
        qCWarning(categoryUIRadioStationsManagerModel) << "Invalid radio station id:" << radioStationId;
        return;
    }

    const auto& url = m_model->data(radioStationId, "url").toString();
    if(url.isEmpty()){
        qCWarning(categoryUIRadioStationsManagerModel) << "Empty URL for radio station id:" << radioStationId;
        return;
    }

    const auto& name = m_model->data(radioStationId, "name").toString();
    if(name.isEmpty()){
        qCWarning(categoryUIRadioStationsManagerModel) << "Empty URL for radio station id:" << radioStationId;
        return;
    }

    m_currentRadioStationId = radioStationId;

    QVariantMap data;

    data[app::radio::Play::Name] = name;
    data[app::radio::Play::URL] = url;

    emit signalUCommand(app::radio::Play::__name__, data);
}
void UIRadioStationsManager::play()
{
    if(m_currentRadioStationId == std::numeric_limits<quint64>::max()){
        qCWarning(categoryUIRadioStationsManagerModel) << "No radio station selected";
        return;
    }

    play(m_currentRadioStationId);
}
void UIRadioStationsManager::pause()
{

}
void UIRadioStationsManager::stop()
{
    emit signalUCommand(app::radio::Stop::__name__);
}

void UIRadioStationsManager::handleUpdateRadioModelPointer(const QVariantMap& data)
{
    ParameterHandler ph(data);

    std::shared_ptr<UTableModel> radioModelPointer;

    if(!ph.handle(radioModelPointer, app::model::RadioModel::Model)){
        qCWarning(categoryUIRadioStationsManagerModel) << "Failed to handle radio model. Data:" << data;
        return;
    }

    m_model = radioModelPointer;

    m_proxyModel.setSourceModel(m_model.get());

    m_proxyModel.setSortFunction("name", [](const QVariant& a, const QVariant& b) {
        return a.toString() < b.toString();
    });
    m_proxyModel.setSortFunction("id", [](const QVariant& a, const QVariant& b) {
        return a.toULongLong() < b.toULongLong();
    });

    m_proxyModel.sort("id", Qt::AscendingOrder);
}

USortFilterProxyTableModel* UIRadioStationsManager::proxyModel()
{
    return &m_proxyModel;
}
