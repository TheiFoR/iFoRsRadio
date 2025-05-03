#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>

#include "src/enums/pages.h"
#include "src/interface/uinterface.h"
#include "src/managers/ui/radio/uiradiostationsmanager.h"
#include "src/managers/ui/server/uiservermanager.h"
#include "src/models/list/ulistmodel.h"

class UIManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(Pages::Page currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged FINAL)

    Q_PROPERTY(UIServerManager* server READ server CONSTANT FINAL)
    Q_PROPERTY(UIRadioStationsManager* radioStations READ radioStations CONSTANT FINAL)

public:
    explicit UIManager(QObject *parent = nullptr);

    void registrationSubscribe() override;

    Pages::Page currentPage() const;
    void setCurrentPage(const Pages::Page &newCurrentPage);

    UIServerManager* server();
    UIRadioStationsManager* radioStations();

signals:
    void currentPageChanged();

private:
    Pages::Page m_currentPage = Pages::RadioStations;

    UIServerManager m_server;
    UIRadioStationsManager m_radioStations;
};

#endif // UIMANAGER_H
