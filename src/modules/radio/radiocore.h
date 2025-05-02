#ifndef RADIOCORE_H
#define RADIOCORE_H

#include <QObject>

#include "src/interface/uinterface.h"
#include "src/models/list/ulistmodel.h"

class RadioCore : public UInterface
{
    Q_OBJECT
public:
    explicit RadioCore(QObject *parent = nullptr);

    void registrationSubscribe() override;

signals:
private:
    UListModel m_radioModel;
};

#endif // RADIOCORE_H
