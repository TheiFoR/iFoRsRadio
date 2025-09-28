// apistreamreader.cpp
#include "apistreamreader.h"
#include <QDebug>

ApiStreamReader::ApiStreamReader(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &ApiStreamReader::fetchNowPlaying);
    connect(&m_manager, &QNetworkAccessManager::finished, this, &ApiStreamReader::onReplyFinished);
}

void ApiStreamReader::setStationId(int stationId)
{
    m_stationId = stationId;
}

void ApiStreamReader::setApiBaseUrl(const QUrl &baseUrl)
{
    m_apiBaseUrl = baseUrl;
}

void ApiStreamReader::start(int intervalMs)
{
    if (m_stationId < 0 || !m_apiBaseUrl.isValid()) {
        qWarning() << "ApiStreamReader: invalid stationId or API base URL";
        return;
    }
    fetchNowPlaying();
    m_timer.start(intervalMs);
}

void ApiStreamReader::stop()
{
    m_timer.stop();
}

void ApiStreamReader::fetchNowPlaying()
{
    QUrl url = m_apiBaseUrl;
    url.setPath(QString("/api/nowplaying/%1").arg(m_stationId));
    QNetworkRequest request(url);
    m_manager.get(request);
}

void ApiStreamReader::onReplyFinished(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "ApiStreamReader: network error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    if (!obj.contains("now_playing")) return;

    QJsonObject now = obj["now_playing"].toObject();
    if (!now.contains("song")) return;

    QJsonObject song = now["song"].toObject();

    TrackInfo info;
    info.title = song["title"].toString();
    info.artist = song["artist"].toString();
    info.album = song["album"].toString();
    info.stationName = obj["station"].toObject()["name"].toString();
    info.genre = song["genre"].toString();
    info.bitrate = obj["station"].toObject()["mounts"].toArray().first().toObject()["bitrate"].toInt();
    info.duration = now["duration"].toInt();
    info.songId = song["id"].toString();
    info.art = song["art"].toString();
    info.elapsed = now["elapsed"].toInt();
    info.remaining = now["remaining"].toInt();
    info.playlist = now["playlist"].toString();

    if (info != m_lastInfo) {
        m_lastInfo = info;
        emit trackChanged(info);
    }
}
