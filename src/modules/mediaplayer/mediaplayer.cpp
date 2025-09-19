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

    emit subscribe(app::mediaPlayer::PlayerPlay::__name__, this, std::bind(&MediaPlayer::handlePlay, this, std::placeholders::_1));
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

void MediaPlayer::handlePlay(const QVariantMap &data)
{
    ParameterHandler ph(data);

    QString name;
    QUrl url;

    if(!ph.handle(name, app::mediaPlayer::PlayerPlay::Name)){
        qCWarning(categoryMediaPlayerHandlePlay) << "Failed to handle name. Data:" << data;
        return;
    }

    if(!ph.handle(url, app::mediaPlayer::PlayerPlay::URL)){
        qCWarning(categoryMediaPlayerHandlePlay) << "Failed to handle URL. Data:" << data;
        return;
    }

    if(!url.isValid()){
        qCWarning(categoryMediaPlayerHandlePlay) << "Invalid URL:" << url;
        return;
    }

    qCInfo(categoryMediaPlayerHandlePlay) << "Playing media:";
    qCInfo(categoryMediaPlayerHandlePlay) << "Name:" << name;
    qCInfo(categoryMediaPlayerHandlePlay) << "URL:" << url.toString();

    stop();

    setSource(url);

    play();
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
