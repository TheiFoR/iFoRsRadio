#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>

#include "src/enums/pages.h"
#include "src/interface/uinterface.h"
#include "src/managers/ui/client/uiclientmanager.h"
#include "src/managers/ui/radio/uiradiostationsmanager.h"
#include "src/managers/ui/server/uiservermanager.h"
#include "src/managers/ui/mediaplayer/uimediaplayermanager.h"
#include "src/models/list/ulistmodel.h"
#include "src/version.h"

class UIManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(QString appVersion READ appVersion CONSTANT FINAL)
    Q_PROPERTY(QString appName READ appName CONSTANT FINAL)

    Q_PROPERTY(Pages::Page currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged FINAL)

    Q_PROPERTY(quint8 styleIndex READ styleIndex WRITE setStyleIndex NOTIFY styleIndexChanged FINAL)

    Q_PROPERTY(UIServerManager* server READ server CONSTANT FINAL)
    Q_PROPERTY(UIClientManager* client READ client CONSTANT FINAL)
    Q_PROPERTY(UIRadioStationsManager* radioStations READ radioStations CONSTANT FINAL)
    Q_PROPERTY(UIMediaPlayerManager* mediaplayer READ mediaplayer CONSTANT FINAL)

public:
    explicit UIManager(QObject *parent = nullptr);
    ~UIManager();

    void registrationSubscribe() override;

    Pages::Page currentPage() const;
    void setCurrentPage(const Pages::Page &newCurrentPage);

    UIServerManager* server();
    UIClientManager* client();
    UIRadioStationsManager* radioStations();
    UIMediaPlayerManager* mediaplayer();

    quint8 styleIndex() const;
    void setStyleIndex(quint8 newStyleIndex);

    QString appVersion() const;
    QString appName() const;

signals:
    void currentPageChanged();
    void styleIndexChanged();

private:
    const QString m_appVersion = PROJECT_FULL_VERSION;
    const QString m_appName = PROJECT_NAME;

    Pages::Page m_currentPage = Pages::RadioStations;

    quint8 m_styleIndex = 0;

    UIServerManager m_server{this};
    UIClientManager m_client{this};
    UIRadioStationsManager m_radioStations{this};
    UIMediaPlayerManager m_mediaplayer{this};
};

#endif // UIMANAGER_H
