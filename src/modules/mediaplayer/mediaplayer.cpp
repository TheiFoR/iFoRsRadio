#include "mediaplayer.h"

LOG_DECLARE(MediaPlayer, Volume)
LOG_DECLARE(MediaPlayer, Registration)
LOG_DECLARE(MediaPlayer, HandlePlay)
LOG_DECLARE(MediaPlayer, HandleVolume)
LOG_DECLARE(MediaPlayer, Module)
LOG_DECLARE(MediaPlayer, Core)

MediaPlayer::MediaPlayer(QObject *parent)
    : UInterface{parent}
{
    m_player.setAudioOutput(new QAudioOutput(this));

    connect(&m_player, &QMediaPlayer::mediaStatusChanged, this, &MediaPlayer::onMediaStatusChanged);
    connect(&m_player, &QMediaPlayer::playbackStateChanged, this, &MediaPlayer::onMediaPlaybackChanged);
}

MediaPlayer::~MediaPlayer()
{
    qCInfo(categoryMediaPlayerCore) << "Destroy";
    Config::setValue("MediaPlayer", "Volume", m_volume);
    disconnect(this, nullptr, nullptr, nullptr);
    qCInfo(categoryMediaPlayerCore) << "Destroy complete";
}

void MediaPlayer::registrationSubscribe()
{
    qCInfo(categoryMediaPlayerRegistration) << "Registration subscription started";

    emit createSubscribe(app::mediaPlayer::PlayerVolume::__name__, this);
    emit createSubscribe(app::mediaPlayer::PlayerStateChanged::__name__, this);

    emit subscribe(app::mediaPlayer::PlayerPlay::__name__, this, std::bind(&MediaPlayer::handlePlay, this, std::placeholders::_1));
    emit subscribe(app::mediaPlayer::PlayerPause::__name__, this, std::bind(&MediaPlayer::handlePause, this, std::placeholders::_1));
    emit subscribe(app::mediaPlayer::PlayerStop::__name__, this, std::bind(&MediaPlayer::handleStop, this, std::placeholders::_1));
    emit subscribe(app::mediaPlayer::PlayerVolume::__name__, this, std::bind(&MediaPlayer::handleVolume, this, std::placeholders::_1));

    qCInfo(categoryMediaPlayerRegistration) << "Registration subscription completed";

    emit done(this);
}

void MediaPlayer::start()
{
    qCInfo(categoryMediaPlayerModule) << "Module has been started";

    setVolume(Config::getValue("MediaPlayer", "Volume", m_volume));
    sendVolume();
}

void MediaPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qCInfo(categoryMediaPlayerCore) << "Media status changed:" << status;

    // PlayStates::State state;
    // switch (status) {
    // case QMediaPlayer::NoMedia:
    //     state = PlayStates::Empty;
    //     break;
    // case QMediaPlayer::LoadingMedia:
    //     state = PlayStates::Loading;
    //     break;
    // case QMediaPlayer::LoadedMedia:
    //     state = PlayStates::Ready;
    //     break;
    // case QMediaPlayer::StalledMedia:
    //     state = PlayStates::Loading;
    //     break;
    // case QMediaPlayer::BufferingMedia:
    //     state = PlayStates::Loading;
    //     break;
    // case QMediaPlayer::BufferedMedia:
    //     state = PlayStates::Playing;
    //     break;
    // case QMediaPlayer::EndOfMedia:
    //     state = PlayStates::Error;
    //     break;
    // case QMediaPlayer::InvalidMedia:
    //     state = PlayStates::Error;
    //     break;
    // }

    // if(!m_id){
    //     qCWarning(categoryMediaPlayerCore) << "Media ID is not set, cannot send state change";
    //     return;
    // }

    // QVariantMap data;

    // data[app::mediaPlayer::PlayerStateChanged::Id] = m_id.value();
    // data[app::mediaPlayer::PlayerStateChanged::State] = state;

    // emit signalUCommand(app::mediaPlayer::PlayerStateChanged::__name__, data);
}

void MediaPlayer::onMediaPlaybackChanged(QMediaPlayer::PlaybackState state)
{
    qCInfo(categoryMediaPlayerCore) << "Media playback state changed:" << state;

    PlayStates::State playState;
    switch (state) {
    case QMediaPlayer::PlaybackState::StoppedState:
        playState = PlayStates::Stopped;
        break;
    case QMediaPlayer::PlaybackState::PlayingState:
        playState = PlayStates::Playing;
        break;
    case QMediaPlayer::PlaybackState::PausedState:
        playState = PlayStates::Paused;
        break;
    }

    if(!m_id){
        qCWarning(categoryMediaPlayerCore) << "Media ID is not set, cannot send state change";
        return;
    }

    QVariantMap data;

    data[app::mediaPlayer::PlayerStateChanged::Id] = m_id.value();
    data[app::mediaPlayer::PlayerStateChanged::State] = playState;

    emit signalUCommand(app::mediaPlayer::PlayerStateChanged::__name__, data);
}

void MediaPlayer::handlePlay(const QVariantMap &data)
{
    ParameterHandler ph(data);

    std::optional<quint64> id;
    std::optional<QString> name;
    std::optional<QUrl> url;

    ph.handle<ParameterHandler::Optional>(id, app::mediaPlayer::PlayerPlay::Id);
    ph.handle<ParameterHandler::Optional>(name, app::mediaPlayer::PlayerPlay::Name);
    ph.handle<ParameterHandler::Optional>(url, app::mediaPlayer::PlayerPlay::URL);

    if(url && !url.value().isValid()){
        qCWarning(categoryMediaPlayerHandlePlay) << "Invalid URL:" << url;
        return;
    }

    if(name && url && id){
        qCInfo(categoryMediaPlayerHandlePlay) << "Playing media:";
        qCInfo(categoryMediaPlayerHandlePlay) << "Id:" << id.value();
        qCInfo(categoryMediaPlayerHandlePlay) << "Name:" << name.value();
        qCInfo(categoryMediaPlayerHandlePlay) << "URL:" << url.value().toString();

        qCDebug(categoryMediaPlayerHandlePlay) << "Setting media ID to" << id.value();
        m_id = id;
    }
    else{
        qCInfo(categoryMediaPlayerHandlePlay) << "Resuming media playback";
    }

    if(url){
        stop();
        setSource(url.value());
    }

    play();
}

void MediaPlayer::handlePause(const QVariantMap &data)
{
    Q_UNUSED(data)

    qCInfo(categoryMediaPlayerHandlePlay) << "Pausing media playback";

    pause();
}

void MediaPlayer::handleStop(const QVariantMap &data)
{
    Q_UNUSED(data)

    qCInfo(categoryMediaPlayerHandlePlay) << "Stopping media playback";

    stop();
}

void MediaPlayer::handleVolume(const QVariantMap &data)
{
    ParameterHandler ph(data);

    float volume;

    if(!ph.handle(volume, app::mediaPlayer::PlayerVolume::Volume)){
        qCWarning(categoryMediaPlayerHandleVolume) << "Failed to handle volume. Data:" << data;
        return;
    }
    if(volume < 0.0f || volume > 1.0f){
        qCWarning(categoryMediaPlayerHandleVolume) << "Invalid volume value:" << volume << " Volume must be between 0.0 and 1.0";
        return;
    }

    setVolume(volume);
}

void MediaPlayer::setSource(const QUrl &url)
{
    m_player.setSource(url);
}

void MediaPlayer::setVolume(float volume)
{
    if(!m_player.audioOutput()){
        qCWarning(categoryMediaPlayerVolume) << "Audio output is not set, cannot set volume";
        return;
    }
    if(volume < 0.0f || volume > 1.0f){
        qCWarning(categoryMediaPlayerVolume) << "Invalid volume value:" << volume << " Volume must be between 0.0 and 1.0";
        return;
    }

    m_volume = volume;

    m_player.audioOutput()->setVolume(m_volume);
}

void MediaPlayer::sendVolume()
{
    QVariantMap data;
    data[app::mediaPlayer::PlayerVolume::Volume] = m_volume;
    emit signalUCommand(app::mediaPlayer::PlayerVolume::__name__, data);
}

void MediaPlayer::play()
{
    if (m_player.playbackState() == QMediaPlayer::PlaybackState::PlayingState) {
        qCDebug(categoryMediaPlayerHandlePlay) << "Media player is already playing";
        return;
    }
    if (m_player.source().isEmpty()) {
        qCInfo(categoryMediaPlayerHandlePlay) << "No media source set, cannot play";
        return;
    }
    m_player.play();
}

void MediaPlayer::stop()
{
    if (m_player.playbackState() == QMediaPlayer::PlaybackState::StoppedState) {
        qCDebug(categoryMediaPlayerHandlePlay) << "Media player is already stopped";
        return;
    }
    m_player.stop();
}

void MediaPlayer::pause()
{
    if (m_player.playbackState() == QMediaPlayer::PlaybackState::PausedState) {
        qCDebug(categoryMediaPlayerHandlePlay) << "Media player is already paused";
        return;
    }
    m_player.pause();
}
