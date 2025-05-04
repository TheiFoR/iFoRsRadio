#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMutex>

#include "src/core/core.h"
#include "src/enums/pages.h"
#include "src/enums/connectionstatus.h"

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    static int maxCategoryWidth = 0;
    static int maxSubcategoryWidth = 0;
    static QMutex mutex;

    QStringList parts = QString(context.category).split("][");
    QString category = parts.value(0);
    QString subcategory = parts.value(1);

    {
        QMutexLocker locker(&mutex);
        if (category.length() > maxCategoryWidth)
            maxCategoryWidth = category.length();
        if (subcategory.length() > maxSubcategoryWidth)
            maxSubcategoryWidth = subcategory.length();
    }

    auto centerText = [](const QString& text, int width) -> QString {
        int padding = width - text.length();
        int left = padding / 2;
        int right = padding - left;
        return QString(left, ' ') + text + QString(right, ' ');
    };

    // Определяем префикс по уровню
    QChar levelChar;
    switch (type) {
    case QtDebugMsg:    levelChar = 'D'; break;
    case QtInfoMsg:     levelChar = 'I'; break;
    case QtWarningMsg:  levelChar = 'W'; break;
    case QtCriticalMsg: levelChar = 'E'; break;
    case QtFatalMsg:    levelChar = 'F'; break;
    default:            levelChar = '?'; break;
    }

    QString time = QTime::currentTime().toString("HH:mm:ss.zzz");

    QString formattedCategory, formattedSubcategory;
    {
        QMutexLocker locker(&mutex);
        formattedCategory = "[" + centerText(category, maxCategoryWidth) + "]";
        formattedSubcategory = "[" + centerText(subcategory, maxSubcategoryWidth) + "]";
    }

    QTextStream(stderr) << "[" << time << "] "
                        << levelChar << " "
                        << formattedCategory << formattedSubcategory << " "
                        << msg << '\n';
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
