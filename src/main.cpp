#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "src/core/core.h"
#include "src/enums/pages.h"
#include "src/enums/connectionstatus.h"

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "[%s] %s\n", context.category, localMsg.constData());
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qRegisterMetaType<Pages::Page>("Pages");
    qmlRegisterUncreatableType<Pages>("Enums", 1, 0, "Pages", "Not creatable as it is an enum type");

    qRegisterMetaType<ConnectionStatus>("ConnectionStatuses");
    qmlRegisterUncreatableType<ConnectionStatuses>("Enums", 1, 0, "ConnectionStatuses", "Not creatable as it is an enum type");

    Core core;

    engine.rootContext()->setContextProperty("core", core.uiManager());

    core.start();


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
