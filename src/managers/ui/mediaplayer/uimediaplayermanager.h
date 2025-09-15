#ifndef UIMEDIAPLAYERMANAGER_H
#define UIMEDIAPLAYERMANAGER_H

#include "src/interface/uinterface.h"
#include "src/enums/playstates.h"

class UIMediaPlayerManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(PlayStates::State currentState READ currentState WRITE setCurrentState NOTIFY currentStateChanged FINAL)

    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(QString currentTitle READ currentTitle WRITE setCurrentTitle NOTIFY currentTitleChanged FINAL)

public:
    explicit UIMediaPlayerManager(QObject *parent = nullptr);

    void registrationSubscribe() override;

    PlayStates::State currentState() const;
    float volume() const;
    QString currentTitle() const;

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
    void setVolume(float newVolume);
    void setCurrentTitle(const QString &newCurrentTitle);
};

#endif // UIMEDIAPLAYERMANAGER_H
