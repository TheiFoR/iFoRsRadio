#ifndef PAGES_H
#define PAGES_H

#include <QObject>

class Pages : public QObject {
    Q_GADGET

public:
    enum Page {
        Home = 0,
        Rooms = 1,
        Settings = 2,
    };

    Q_ENUM(Page)
};

#endif // PAGES_H
