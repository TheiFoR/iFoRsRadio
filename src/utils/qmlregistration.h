#ifndef QMLREGISTRATION_H
#define QMLREGISTRATION_H

#include <QObject>
#include <QtQml>
#include <QQmlContext>

class QmlRegistration : public QObject
{
    Q_OBJECT
public:
    explicit QmlRegistration(QQmlApplicationEngine* engine, QObject *parent = nullptr);

    template<typename T>
    void registerEnum(const QString& uri, const QString& qmlName)
    {
        qmlRegisterUncreatableType<T>(
            uri.toUtf8().constData(),
            1,
            0,
            qmlName.toUtf8().constData(),
            "Enum only"
            );
    }

    void registerContext(const QString& name, QObject* obj);

signals:
private:
    QQmlApplicationEngine* m_engine = nullptr;
};

#endif // QMLREGISTRATION_H
