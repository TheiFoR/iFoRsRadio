#include "radiocore.h"

RadioCore::RadioCore(QObject *parent)
    : UInterface{parent}
{
    m_radioModel.setRoleNames({
        "id",
        "name",
        "url",
    });

    m_radioModel.appendItem({
        {"id", 0},
        {"name", "Country"},
        {"url", "https://icepool.silvacast.com/COUNTRY108.mp3"},
    });
    m_radioModel.appendItem({
        {"id", 1},
        {"name", "Pop"},
        {"url", "https://cloud.revma.ihrhls.com/zc185?rj-org=n3ab-e2&rj-ttl=5&rj-tok=AAABljAD1_gA-dzvZ9ZYrBu3NA"},
    });
}

void RadioCore::registrationSubscribe()
{
    emit createSubscribe(app::model::RadioModel::__name__, this);

    QVariantMap data;
    data[app::model::RadioModel::Model] = QVariant::fromValue(&m_radioModel);

    emit signalUCommand(app::model::RadioModel::__name__, data);
}
