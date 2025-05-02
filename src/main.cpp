#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/core/core.h"
#include "src/utils/qmlregistration.h"
#include "src/enums/pages.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Core core;
    QmlRegistration qmlRegistration(&engine);

    qmlRegistration.registerEnum<Pages>("Enums", "Pages");
    qmlRegistration.registerContext("core", core.uiManager());

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
