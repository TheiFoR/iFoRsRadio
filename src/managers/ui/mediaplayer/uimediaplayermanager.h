#ifndef UIMEDIAPLAYERMANAGER_H
#define UIMEDIAPLAYERMANAGER_H

#include "api/internal/mediaPlayer.h"

#include "src/interface/uinterface.h"
#include "src/enums/playstates.h"
#include "src/types/logdef.h"

class UIMediaPlayerManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(PlayStates::State currentState READ currentState NOTIFY currentStateChanged FINAL)
    Q_PROPERTY(QString currentTitle READ currentTitle NOTIFY currentTitleChanged FINAL)

    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged FINAL)

public:
    explicit UIMediaPlayerManager(QObject *parent = nullptr);
    ~UIMediaPlayerManager();

    void registrationSubscribe() override;

    PlayStates::State currentState() const;
    QString currentTitle() const;
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
    void currentTitleChanged();

    void mutedChanged();

private:
    PlayStates::State m_currentState = PlayStates::Stopped;
    float m_volume = 1.0f;
    float m_tempVolume = 1.0f;
    QString m_currentTitle = "iFoR's Radio";
    bool m_muted = false;

    void setCurrentState(const PlayStates::State &newCurrentState);
    void setCurrentTitle(const QString &newCurrentTitle);
    void sendVolume();

    void handleVolume(const QVariantMap& data);
    void handleStateChanged(const QVariantMap& data);
};

#endif // UIMEDIAPLAYERMANAGER_H
