#ifndef PLAYSTATES_H
#define PLAYSTATES_H

#include <QObject>
#include <QtQml>

class PlayStates : public QObject {
    Q_GADGET

public:
    enum State {
        Ready,
        Playing,
        Paused,
        Loading,
    };

    Q_ENUM(State)

    static void declareQml() {
        qRegisterMetaType<PlayStates::State>("PlayStates");
        qmlRegisterUncreatableType<PlayStates>("Enums", 1, 0, "PlayStates", "Not creatable as it is an enum type");
    }
};

#endif // PLAYSTATES_H
