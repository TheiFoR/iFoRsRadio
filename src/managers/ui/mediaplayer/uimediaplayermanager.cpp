#include "uimediaplayermanager.h"
#include "src/utils/parameterhandler.h"

LOG_DECLARE(UIMediaPlayer, Registration)
LOG_DECLARE(UIMediaPlayer, HandleVolume)
LOG_DECLARE(UIMediaPlayer, Core)

UIMediaPlayerManager::UIMediaPlayerManager(QObject *parent)
    : UInterface{parent}
{

}

UIMediaPlayerManager::~UIMediaPlayerManager()
{
    qCInfo(categoryUIMediaPlayerCore) << "Destroy";
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categoryUIMediaPlayerCore) << "Destroy complete";
}

void UIMediaPlayerManager::registrationSubscribe()
{
    qCInfo(categoryUIMediaPlayerRegistration) << "Registration subscription started";

    emit createSubscribe(app::mediaPlayer::PlayerVolume::__name__, this);

    emit subscribe(app::mediaPlayer::PlayerVolume::__name__, this, std::bind(&UIMediaPlayerManager::handleVolume, this, std::placeholders::_1));
    emit subscribe(app::mediaPlayer::PlayerStateChanged::__name__, this, std::bind(&UIMediaPlayerManager::handleStateChanged, this, std::placeholders::_1));

    qCInfo(categoryUIMediaPlayerRegistration) << "Registration subscription completed";

    emit done(this);
}

PlayStates::State UIMediaPlayerManager::currentState() const
{
    return m_currentState;
}

void UIMediaPlayerManager::play()
{
    emit signalUCommand(app::mediaPlayer::PlayerPlay::__name__);
}

void UIMediaPlayerManager::pause()
{    
    emit signalUCommand(app::mediaPlayer::PlayerPause::__name__);
}

void UIMediaPlayerManager::stop()
{
    emit signalUCommand(app::mediaPlayer::PlayerStop::__name__);
}

void UIMediaPlayerManager::setCurrentState(const PlayStates::State &newCurrentState)
{
    if (m_currentState == newCurrentState)
        return;
    m_currentState = newCurrentState;
    emit currentStateChanged();
}

float UIMediaPlayerManager::volume() const
{
    return m_volume;
}

void UIMediaPlayerManager::setVolume(float newVolume)
{
    if (qFuzzyCompare(m_volume, newVolume))
        return;
    m_volume = newVolume;
    emit volumeChanged();

    sendVolume();
}

void UIMediaPlayerManager::sendVolume()
{
    QVariantMap data;
    data[app::mediaPlayer::PlayerVolume::Volume] = m_volume;
    emit signalUCommand(app::mediaPlayer::PlayerVolume::__name__, data);
}

QString UIMediaPlayerManager::currentTitle() const
{
    return m_currentTitle;
}

void UIMediaPlayerManager::setCurrentTitle(const QString &newCurrentTitle)
{
    if (m_currentTitle == newCurrentTitle)
        return;
    m_currentTitle = newCurrentTitle;
    emit currentTitleChanged();
}

void UIMediaPlayerManager::handleVolume(const QVariantMap &data)
{
    qCInfo(categoryUIMediaPlayerHandleVolume) << "Handling volume command. Data:" << data;
    ParameterHandler ph(data);

    float volume;
    if(!ph.handle(volume, app::mediaPlayer::PlayerVolume::Volume)){
        qCWarning(categoryUIMediaPlayerHandleVolume) << "Failed to handle volume. Data:" << data;
        return;
    }
    if(volume < 0.0f || volume > 1.0f){
        qCWarning(categoryUIMediaPlayerHandleVolume) << "Invalid volume value:" << volume << "Expected range: 0.0 - 1.0";
        return;
    }

    m_volume = volume;
    emit volumeChanged();
}

void UIMediaPlayerManager::handleStateChanged(const QVariantMap &data)
{
    qCInfo(categoryUIMediaPlayerCore) << "Handling state changed command.";
    ParameterHandler ph(data);

    quint64 id;
    PlayStates::State state;

    if(!ph.handle(id, app::mediaPlayer::PlayerStateChanged::Id)){
        qCWarning(categoryUIMediaPlayerCore) << "Failed to handle id. Data:" << data;
        return;
    }
    if(!ph.handle(state, app::mediaPlayer::PlayerStateChanged::State)){
        qCWarning(categoryUIMediaPlayerCore) << "Failed to handle state. Data:" << data;
        return;
    }

    qCInfo(categoryUIMediaPlayerCore) << "Media player state changed. ID:" << id << " State:" << state;
    setCurrentState(state);
}

bool UIMediaPlayerManager::muted() const
{
    return m_muted;
}

void UIMediaPlayerManager::setMuted(bool newMuted)
{
    if (m_muted == newMuted)
        return;

    if(newMuted){
        m_tempVolume = m_volume;
        setVolume(0.0f);
    }
    else{
        setVolume(m_tempVolume);
    }

    m_muted = newMuted;
    emit mutedChanged();
}
