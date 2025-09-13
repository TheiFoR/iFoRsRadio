#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "api/internal/mediaPlayer.h"

#include "src/interface/uinterface.h"
#include "src/utils/parameterhandler.h"

class MediaPlayer : public UInterface
{
    Q_OBJECT

public:
    explicit MediaPlayer(QObject *parent = nullptr);

    void registrationSubscribe() override;

signals:

private:
    QMediaPlayer m_player{this};

    void handlePlay(const QVariantMap &data);
    void handleStop(const QVariantMap &data);
    void handleVolume(const QVariantMap &data);

    void setSource(const QUrl &url);
    void setVolume(float volume);

    void play();
    void stop();
};

#endif // MEDIAPLAYER_H
