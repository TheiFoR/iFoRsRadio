// apistreamreader.h
#ifndef APISTREAMREADER_H
#define APISTREAMREADER_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "src/modules/mediaplayer/trackinfo.h"

class ApiStreamReader : public QObject
{
    Q_OBJECT
public:
    explicit ApiStreamReader(QObject* parent = nullptr);

    void setStationId(int stationId);
    void setApiBaseUrl(const QUrl& baseUrl);
    void start(int intervalMs = 1000);
    void stop();

signals:
    void trackChanged(const TrackInfo& info);

private slots:
    void fetchNowPlaying();
    void onReplyFinished(QNetworkReply* reply);

private:
    QUrl m_apiBaseUrl;
    int m_stationId = -1;
    QTimer m_timer{this};
    QNetworkAccessManager m_manager{this};
    TrackInfo m_lastInfo;
};

#endif // APISTREAMREADER_H
