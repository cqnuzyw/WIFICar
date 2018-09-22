#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "network.h"
#include <QQmlContext>

int main(int argc, char **argv)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    NetWork *network=new NetWork();
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("network",network);
    engine.addImageProvider(QLatin1String("codeimg"),network->pImgProvider());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
