#ifndef CONNECTIONSTATUS_H
#define CONNECTIONSTATUS_H

#include <QObject>

class ConnectionStatuses : public QObject {
    Q_OBJECT
public:
    enum Status {
        NoConnection,
        Connecting,
        Connected
    };
    Q_ENUM(Status);

    static QString toString(Status status){
        switch (status) {
        case NoConnection:
            return "No connection";
        case Connecting:
            return "Connecting";
        case Connected:
            return "Connected";
        }
    }
};

typedef ConnectionStatuses::Status ConnectionStatus;

Q_DECLARE_METATYPE(ConnectionStatus)

#endif // CONNECTIONSTATUS_H
