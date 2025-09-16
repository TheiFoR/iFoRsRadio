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

public:
    explicit UIMediaPlayerManager(QObject *parent = nullptr);

    void registrationSubscribe() override;

    PlayStates::State currentState() const;
    QString currentTitle() const;
    float volume() const;
    void setVolume(float newVolume);


    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();

signals:
    void currentStateChanged();
    void volumeChanged();
    void currentTitleChanged();

private:
    PlayStates::State m_currentState = PlayStates::Ready;
    float m_volume = 1.0f;
    QString m_currentTitle = "iFoR's Radio";

    void setCurrentState(const PlayStates::State &newCurrentState);
    void setCurrentTitle(const QString &newCurrentTitle);
    void sendVolume();

    void handleVolume(const QVariantMap& data);
};

#endif // UIMEDIAPLAYERMANAGER_H
