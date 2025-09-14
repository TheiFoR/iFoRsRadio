#ifndef PAGES_H
#define PAGES_H

#include <QObject>
#include <QtQml>

class Pages : public QObject {
    Q_GADGET

public:
    enum Page {
        RadioStations = 0,
        Rooms = 1,
        Settings = 2,
    };

    Q_ENUM(Page)

    static void declareQml() {
        qRegisterMetaType<Pages::Page>("Pages");
        qmlRegisterUncreatableType<Pages>("Enums", 1, 0, "Pages", "Not creatable as it is an enum type");
    }
};

#endif // PAGES_H
