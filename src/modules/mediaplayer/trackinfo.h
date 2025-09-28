#ifndef TRACKINFO_H
#define TRACKINFO_H

#include <QObject>
#include <QVariant>

#include <QObject>
#include <QString>

// trackinfo.h (дополнено)
struct TrackInfo
{
    Q_GADGET
    Q_PROPERTY(QString title MEMBER title)
    Q_PROPERTY(QString artist MEMBER artist)
    Q_PROPERTY(QString album MEMBER album)
    Q_PROPERTY(QString stationName MEMBER stationName)
    Q_PROPERTY(QString genre MEMBER genre)
    Q_PROPERTY(int bitrate MEMBER bitrate)
    Q_PROPERTY(qint64 duration MEMBER duration)
    Q_PROPERTY(QString songId MEMBER songId)
    Q_PROPERTY(QString art MEMBER art)
    Q_PROPERTY(int elapsed MEMBER elapsed)
    Q_PROPERTY(int remaining MEMBER remaining)
    Q_PROPERTY(QString playlist MEMBER playlist)

public:
    QString title;
    QString artist;
    QString album;
    QString stationName;
    QString genre;
    int bitrate = 0;
    qint64 duration = 0;
    QString songId;
    QString art;
    int elapsed = 0;
    int remaining = 0;
    QString playlist;

    operator QVariant() const { return QVariant::fromValue(*this); }

    bool operator==(const TrackInfo &other) const {
        return title == other.title
               && artist == other.artist
               && album == other.album
               && stationName == other.stationName
               && genre == other.genre
               && bitrate == other.bitrate
               && duration == other.duration
               && songId == other.songId
               && art == other.art
               && elapsed == other.elapsed
               && remaining == other.remaining
               && playlist == other.playlist;
    }

    bool operator!=(const TrackInfo &other) const { return !(*this == other); }
};

inline QDebug operator<<(QDebug dbg, const TrackInfo &info)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "TrackInfo("
                  << "title:"    << info.title
                  << ", artist:" << info.artist
                  << ", album:"  << info.album
                  << ", bitrate:"  << info.bitrate
                  << ", duration:"   << info.duration
                  << ", songId:"   << info.songId
                  // << ", art: "   << info.art
                  << ", elapsed:"   << info.elapsed
                  << ", remaining:"   << info.remaining
                  << ", playlist:"   << info.playlist
                  << ")";
    return dbg;
}

Q_DECLARE_METATYPE(TrackInfo)

#endif // TRACKINFO_H
