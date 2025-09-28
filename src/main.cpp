#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QMutex>

#include "src/core/core.h"
#include "src/enums/pages.h"
#include "src/enums/playstates.h"
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

    QString levelChar;
    QString msgColor;
    switch (type) {
    case QtDebugMsg:
        levelChar = "\033[37mD\033[0m";
        msgColor = "\033[0;37m";
        break; // Blue
    case QtInfoMsg:
        levelChar = "\033[34mI\033[0m";
        msgColor = "\033[0;34m";
        break; // Green
    case QtWarningMsg:
        levelChar = "\033[33mW\033[0m";
        msgColor = "\033[1;33m";
        break; // Yellow
    case QtCriticalMsg:
        levelChar = "\033[31mE\033[0m";
        msgColor = "\033[1;31m";
        break; // Red
    case QtFatalMsg:
        levelChar = "\033[41mF\033[0m";
        msgColor = "\033[41m";
        break; // Red background
    default:
        levelChar = "\033[37m?\033[0m";
        break; // White
    }

    QString time = QTime::currentTime().toString("HH:mm:ss.zzz");

    QString formattedCategory, formattedSubcategory;
    {
        QMutexLocker locker(&mutex);
        formattedCategory = centerText(category, maxCategoryWidth) + "][" + centerText(subcategory, maxSubcategoryWidth);
    }

    QTextStream(stderr)
        << "\033[0;37m[" << time << "]\033[0m "   // Gray time
        << levelChar << " "
        << "\033[1;37m[" << formattedCategory << "]\033[0m "
        << msgColor << msg << "\033[0m"       // Light gray message
        << '\n';
}



int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Pages::declareQml();
    PlayStates::declareQml();

    app.setWindowIcon(QIcon(":/assets/icons/app/icon.svg"));

    qRegisterMetaType<TrackInfo>("TrackInfo");

    qRegisterMetaType<ConnectionStatus>("ConnectionStatuses");
    qmlRegisterUncreatableType<ConnectionStatuses>("Enums", 1, 0, "ConnectionStatuses", "Not creatable as it is an enum type");

    qmlRegisterSingletonType(QUrl("qrc:/qml/UStyle.qml"), "UStyle", 1, 0, "UStyle");

    Core core;

    engine.rootContext()->setContextProperty("core", core.uiManager());

    core.startCore();


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
