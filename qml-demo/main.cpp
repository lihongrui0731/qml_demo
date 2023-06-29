#include <QApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QFontDatabase>
#include <QFont>
#include "CLineChart.h"
#include "RhythmGradientBar.h"
#include "TimeCursorLabel.h"
#include "InstantSpectrumChart.h"
#include "TimeSpectraChart.h"
#include "scatterchart.h"
#include "filemanager.h"
#include "WebSocketManager.h"


int main(int argc, char *argv[]) {
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QString appConfig{"config_SandV"};
    QApplication app(argc, argv);

    qmlRegisterType<CLineChart>("CLineChart", 1, 0, "CLineChart");
    qmlRegisterType<RhythmGradientBar>("GradientBar", 1, 0, "ChartGradientBar");
    qmlRegisterType<InstantSpectrumChart>("InstantSpectrum", 1, 0, "InstantSpectrumChart");
    qmlRegisterType<TimeSpectraChart>("TimeSpectra", 1, 0, "TimeSpectra");
    qmlRegisterType<TimeCursorLabel>("TimerCursor", 1, 0, "TimeAxis");
    qmlRegisterType<ScatterChart>("ScatterChart", 1, 0, "ScatterChart");

    qmlRegisterType<FileManager>("FileManager", 1, 0, "FileManager");
    qmlRegisterType<WebSocketManager>("WebSocketManager", 1, 0, "WebSocketManager");

    WebSocketManager webSocketManager;

    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("appConfig" ,appConfig);

    qreal dpi = app.devicePixelRatio();

    int font_Id = QFontDatabase::addApplicationFont(":/resources/font/FangZhengHeiTiJianTi.ttf");  //":/冒号表示资源根目录
    QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
    QFont font(font_list[0], 10/dpi, QFont::Black);
    app.setFont(font);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
