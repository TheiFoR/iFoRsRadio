#ifndef UIRADIOSTATIONSMANAGER_H
#define UIRADIOSTATIONSMANAGER_H

#include <QUrl>

#include "api/internal/radio.h"

#include "src/interface/uinterface.h"
#include "src/models/model/usortfilterproxytablemodel.h"
#include "src/models/model/utablemodel.h"
#include "src/utils/parameterhandler.h"

class UIRadioStationsManager : public UInterface
{
    Q_OBJECT

    Q_PROPERTY(USortFilterProxyTableModel* proxyModel READ proxyModel NOTIFY proxyModelChanged FINAL)

public:
    explicit UIRadioStationsManager(QObject *parent = nullptr);
    ~UIRadioStationsManager();

    void registrationSubscribe() override;

    Q_INVOKABLE void play(quint64 radioStationId);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();

    USortFilterProxyTableModel* proxyModel();

signals:
    void proxyModelChanged();

private:
    USortFilterProxyTableModel m_proxyModel{this};
    std::shared_ptr<UTableModel> m_model = nullptr;

    quint64 m_currentRadioStationId = std::numeric_limits<quint64>::max();

    void handleUpdateRadioModelPointer(const QVariantMap &data);
};

#endif // UIRADIOSTATIONSMANAGER_H
