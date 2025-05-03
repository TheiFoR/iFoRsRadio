#include "radiocore.h"

LOG_DECLARE(RadioCore, Core)
LOG_DECLARE(RadioCore, ServerStatus)

RadioCore::RadioCore(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryRadioCoreCore) << "Create";

    m_radioModel.setRoleNames({
        "id",
        "name",
        "url",
        "image",
    });
}

void RadioCore::registrationSubscribe()
{
    qCInfo(categoryRadioCoreCore) << "Registration subscription started";

    emit createSubscribe(app::model::RadioModel::__name__, this);
    emit subscribe(app::server::ServerStatus::__name__, this, std::bind(&RadioCore::handleServerConnectionStatus, this, std::placeholders::_1));
    emit subscribe(api::radio::RadioStationListResponse::__name__, this, std::bind(&RadioCore::handleRadioStationList, this, std::placeholders::_1));

    QVariantMap data;
    data[app::model::RadioModel::Model] = QVariant::fromValue(&m_radioModel);

    emit signalUCommand(app::model::RadioModel::__name__, data);

    qCInfo(categoryRadioCoreCore) << "Registration subscription ended";
}

void RadioCore::handleServerConnectionStatus(const QVariantMap &data)
{
    ParameterHandler ph(data);

    ConnectionStatuses::Status status = ConnectionStatuses::NoConnection;

    if(!ph.handle(status, app::server::ServerStatus::Status)){
        qCWarning(categoryRadioCoreServerStatus) << "Failed to handle connection status. Data:" << data;
        return;
    }

    qCInfo(categoryRadioCoreServerStatus) << "New status:" << ConnectionStatuses::toString(status);

    setServerConnectionStatus(status);

    if(status == ConnectionStatuses::Connected){
        emit signalUPacket(api::radio::RadioStationListRequest::__name__, {});
    }
}

void RadioCore::handleRadioStationList(const QVariantMap &data)
{
    qCInfo(categoryRadioCoreCore) << "New list of radio station!";

    ParameterHandler ph(data);

    QVariantList radioStationList;

    if(!ph.handle(radioStationList, api::radio::RadioStationListResponse::RadioStationList)){
        qCWarning(categoryRadioCoreServerStatus) << "Failed to handle list of radio station. Data:" << data;
        return;
    }

    m_radioModel.clear();

    for(QVariant& radioStationItem : radioStationList){
        QVariantMap radioStationMap = radioStationItem.toMap();

        ph.setParameters(radioStationMap);

        quint64 id;
        QByteArray image;
        QString name;
        QString url;

        if(!ph.handle(id, api::radio::RadioStation::Id)){
            qCWarning(categoryRadioCoreServerStatus) << "Failed to handle image of radio station. Data:" << data;
            return;
        }
        if(!ph.handle(name, api::radio::RadioStation::Name)){
            qCWarning(categoryRadioCoreServerStatus) << "Failed to handle name of radio station. Data:" << data;
            return;
        }
        if(!ph.handle(url, api::radio::RadioStation::URL)){
            qCWarning(categoryRadioCoreServerStatus) << "Failed to handle image of radio station. Data:" << data;
            return;
        }

        ph.handle<ParameterHandler::Optional>(image, api::radio::RadioStation::Image);

        m_radioModel.appendItem({
            {"id", id},
            {"image", image},
            {"name", name},
            {"url", url},
        });
    }

    QVariantMap radioListModel{
        { app::model::RadioModel::Model, QVariant::fromValue(&m_radioModel) }
    };

    emit signalUCommand(app::model::RadioModel::__name__, radioListModel);

    qCInfo(categoryRadioCoreCore) << "Added" << m_radioModel.count() << "radio station!";
}

ConnectionStatuses::Status RadioCore::serverConnectionStatus() const
{
    return m_serverConnectionStatus;
}
void RadioCore::setServerConnectionStatus(const ConnectionStatuses::Status &newServerConnectionStatus)
{
    if (m_serverConnectionStatus == newServerConnectionStatus)
        return;
    m_serverConnectionStatus = newServerConnectionStatus;
    emit serverConnectionStatusChanged();
}
