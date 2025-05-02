#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>

#include "src/enums/pages.h"
#include "src/interface/uinterface.h"
#include "src/models/list/ulistmodel.h"

class UIManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(Pages::Page currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged FINAL)

    Q_PROPERTY(UListModel* radioModel READ radioModel NOTIFY radioModelChanged FINAL)

public:
    explicit UIManager(QObject *parent = nullptr);

    void registrationSubscribe() override;

    Pages::Page currentPage() const;
    void setCurrentPage(const Pages::Page &newCurrentPage);

    UListModel* radioModel();
    void setRadioModel(UListModel* newRadioModel);

public slots:
    void handleUpdateRadioModelPointer(const QVariantMap& data);

signals:
    void currentPageChanged();
    void radioModelChanged();

private:
    Pages::Page m_currentPage = Pages::Page::Home;

    UListModel* m_radioModel = nullptr;

};

#endif // UIMANAGER_H
