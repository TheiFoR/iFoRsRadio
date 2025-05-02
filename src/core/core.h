#ifndef CORE_H
#define CORE_H

#include <QObject>

#include "src/core/connectionmanager.h"
#include "src/interface/uinterface.h"
#include "src/managers/ui/uimanager.h"
#include "src/modules/radio/radiocore.h"

class Core : public UInterface
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);

    UIManager* uiManager();

    void registrationSubscribe() override;

signals:

private:
    ConnectionManager m_connectionManager;

    UIManager m_uiManager;

    RadioCore m_radioCore;
};

#endif // CORE_H
