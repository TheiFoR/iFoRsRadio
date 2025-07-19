#ifndef UIRADIOSTATIONSMANAGER_H
#define UIRADIOSTATIONSMANAGER_H

#include "src/interface/uinterface.h"
#include "src/models/list/ulistmodel.h"
#include "src/utils/parameterhandler.h"

class UIRadioStationsManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(UListModel* model READ model NOTIFY modelChanged FINAL)

public:
    explicit UIRadioStationsManager(QObject *parent = nullptr);

    void registrationSubscribe() override;

    UListModel* model();

signals:
    void modelChanged();

private:
    UListModel* m_model = nullptr;

    void handleUpdateRadioModelPointer(const QVariantMap &data);
};

#endif // UIRADIOSTATIONSMANAGER_H
