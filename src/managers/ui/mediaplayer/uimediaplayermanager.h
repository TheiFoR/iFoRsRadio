#ifndef UIMEDIAPLAYERMANAGER_H
#define UIMEDIAPLAYERMANAGER_H

#include "api/internal/mediaPlayer.h"

#include "src/enums/playstates.h"
#include "src/types/logdef.h"
#include "src/modules/mediaplayer/trackinfo.h"

#include <SubscriptionNode>

class UIMediaPlayerManager : public SubscriptionNode
{
    Q_OBJECT

    Q_PROPERTY(PlayStates::State currentState READ currentState NOTIFY currentStateChanged FINAL)
    Q_PROPERTY(TrackInfo currentTrack READ currentTrack NOTIFY currentTrackChanged FINAL)

    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged FINAL)

public:
    explicit UIMediaPlayerManager(QObject *parent = nullptr);
    ~UIMediaPlayerManager();

    void registrationSubscribe() override;

    PlayStates::State currentState() const;
    TrackInfo currentTrack() const;
    float volume() const;
    void setVolume(float newVolume);


    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

    bool muted() const;
    void setMuted(bool newMuted);

signals:
    void currentStateChanged();
    void volumeChanged();
    void currentTrackChanged();

    void mutedChanged();

private:
    PlayStates::State m_currentState = PlayStates::Stopped;
    float m_volume = 1.0f;
    float m_tempVolume = 1.0f;
    TrackInfo m_currentTrack = TrackInfo();
    bool m_muted = false;

    void setCurrentState(const PlayStates::State &newCurrentState);
    void setCurrentTrack(const TrackInfo &newCurrentTrack);
    void sendVolume();

    void handleVolume(const QVariantMap& data);
    void handleStateChanged(const QVariantMap& data);
    void handleTrackChanged(const QVariantMap& data);
};

#endif // UIMEDIAPLAYERMANAGER_H
