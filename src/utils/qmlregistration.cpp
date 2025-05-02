#include "qmlregistration.h"

QmlRegistration::QmlRegistration(QQmlApplicationEngine* engine, QObject *parent)
    : QObject{parent}
{
    m_engine = engine;
}

void QmlRegistration::registerContext(const QString& name, QObject* obj)
{
    if(m_engine == nullptr){
        return;
    }

    m_engine->rootContext()->setContextProperty(name, obj);
}
