#include "radiocore.h"

LOG_DECLARE(RadioCore, Core)
LOG_DECLARE(RadioCore, ServerStatus)
LOG_DECLARE(RadioCore, Play)

RadioCore::RadioCore(QObject *parent)
    : UInterface{parent}
{
    qCInfo(categoryRadioCoreCore) << "Create";

    m_radioModel = std::make_shared<UTableModel>(this);

    m_radioModel->setColumns({
        "id",
        "name",
        "url",
        "image",
        "genre",
    });
}

RadioCore::~RadioCore()
{
    qCInfo(categoryRadioCoreCore) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categoryRadioCoreCore) << "Destroy complete";
}

void RadioCore::registrationSubscribe()
{
    qCInfo(categoryRadioCoreCore) << "Registration subscription started";

    emit createSubscribe(app::model::RadioModel::__name__, this);
    emit createSubscribe(app::mediaPlayer::PlayerPlay::__name__, this);
    emit createSubscribe(app::mediaPlayer::PlayerStop::__name__, this);

    emit subscribe(app::server::ServerStatus::__name__, this, std::bind(&RadioCore::handleServerConnectionStatus, this, std::placeholders::_1));
    emit subscribe(api::radio::RadioStationListResponse::__name__, this, std::bind(&RadioCore::handleRadioStationList, this, std::placeholders::_1));
    emit subscribe(app::radio::Play::__name__, this, std::bind(&RadioCore::handleRadioPlay, this, std::placeholders::_1));
    emit subscribe(app::radio::Stop::__name__, this, std::bind(&RadioCore::handleRadioStop, this, std::placeholders::_1));

    qCInfo(categoryRadioCoreCore) << "Registration subscription ended";

    emit done(this);
}

void RadioCore::start()
{
    QVariantMap data;
    data[app::model::RadioModel::Model] = QVariant::fromValue(m_radioModel);

    qInfo() << "Update radio model pointer" << m_radioModel.get();

    emit signalUCommand(app::model::RadioModel::__name__, data);
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
        emit signalUPacket(api::radio::RadioStationListRequest::__name__);
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

    m_radioModel->clear();

    for(QVariant& radioStationItem : radioStationList){
        QVariantMap radioStationMap = radioStationItem.toMap();

        ph.setParameters(radioStationMap);

        quint64 id;
        QByteArray image;
        QString name;
        QString url;
        QString genre;

        if(!ph.handle(id, api::radio::RadioStation::Id)){
            qCWarning(categoryRadioCoreServerStatus) << "Failed to handle image of radio station. Data:" << data;
            return;
        }
        if(!ph.handle(name, api::radio::RadioStation::Name)){
            qCWarning(categoryRadioCoreServerStatus) << "Failed to handle name of radio station. Data:" << data;
            return;
        }
        if(!ph.handle(genre, api::radio::RadioStation::Genre)){
            qCWarning(categoryRadioCoreServerStatus) << "Failed to handle genre of radio station. Data:" << data;
            return;
        }
        if(!ph.handle(url, api::radio::RadioStation::URL)){
            qCWarning(categoryRadioCoreServerStatus) << "Failed to handle image of radio station. Data:" << data;
            return;
        }

        ph.handle<ParameterHandler::Optional>(image, api::radio::RadioStation::Image);

        m_radioModel->append(id, {
            {"id", id},
            {"image", image},
            {"name", name},
            {"genre", genre},
            {"url", url},
        });
    }

    qCInfo(categoryRadioCoreCore) << "Added" << m_radioModel->count() << "radio station!";
}

void RadioCore::handleRadioPlay(const QVariantMap &data)
{
    qCInfo(categoryRadioCorePlay) << "Play radio";

    ParameterHandler ph(data);

    quint64 id;
    QString name;
    QUrl url;

    if(!ph.handle(name, app::radio::Play::Name)){
        qCWarning(categoryRadioCorePlay) << "Failed to handle name. Data:" << data;
        return;
    }

    if(!ph.handle(url, app::radio::Play::URL)){
        qCWarning(categoryRadioCorePlay) << "Failed to handle URL. Data:" << data;
        return;
    }

    if(!ph.handle(id, app::radio::Play::Id)){
        qCWarning(categoryRadioCorePlay) << "Failed to handle Id. Data:" << data;
        return;
    }

    qCInfo(categoryRadioCorePlay) << "Id:" << id;
    qCInfo(categoryRadioCorePlay) << "Name:" << name;
    qCInfo(categoryRadioCorePlay) << "URL:" << url;

    if(!url.isValid()){
        qCWarning(categoryRadioCorePlay) << "Invalid URL:" << url;
        return;
    }

    QVariantMap mediaPlayerData;

    mediaPlayerData[app::mediaPlayer::PlayerPlay::Id] = id;
    mediaPlayerData[app::mediaPlayer::PlayerPlay::Name] = name;
    mediaPlayerData[app::mediaPlayer::PlayerPlay::URL] = url;

    emit signalUCommand(app::mediaPlayer::PlayerPlay::__name__, mediaPlayerData);
}

void RadioCore::handleRadioStop(const QVariantMap &data)
{
    Q_UNUSED(data)

    qCInfo(categoryRadioCorePlay) << "Stop radio";

    emit signalUCommand(app::mediaPlayer::PlayerStop::__name__);
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
