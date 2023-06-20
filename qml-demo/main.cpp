#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QFontDatabase>
#include <QFont>
#include "filemanager.h"


int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterType<FileManager>("FileManager", 1, 0, "FileManager");
    QQuickStyle::setStyle("Material");
    QQmlApplicationEngine engine;

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
