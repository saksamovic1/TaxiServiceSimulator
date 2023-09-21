#include "controller.h"
#include "environment.h"
#include "cab.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Cab* cab1 = new Cab(&app);
    Cab* cab2 = new Cab(&app);
    Cab* cab3 = new Cab(&app);

    Controller controller;

    controller.createConnections();

    controller.registerCab(*cab1);
    controller.registerCab(*cab2);
    controller.registerCab(*cab3);

    engine.rootContext()->setContextProperty("cab1", cab1);
    engine.rootContext()->setContextProperty("cab2", cab2);
    engine.rootContext()->setContextProperty("cab3", cab3);

    engine.rootContext()->setContextProperty("env", Environment::getInstance());

    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
