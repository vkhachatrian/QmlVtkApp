#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>

#include <QQuickWindow>

#include "CameraParametersModel.h"
#include "TeethModel.h"
#include "VTKController.h"
#include "AppController.h"
#include "PlaneCutController.h"
#include "SplineCutController.h"
#include "Enums.h"

void registerQmlTypes();

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::GraphicsApi::OpenGL);
    QGuiApplication app(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/Poppins-Regular.ttf");

    QQmlApplicationEngine* engine = new QQmlApplicationEngine();
    VTKController::instance()->setEngine(engine);
    engine->addImportPath(QCoreApplication::applicationDirPath() + "/VtkQml");
    engine->addImportPath("qrc:/qml/qml");

    const QUrl url(QStringLiteral("qrc:/qml/qml/UI/main.qml"));
    QObject::connect(engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    registerQmlTypes();
    engine->load(url);

    return app.exec();
}

void registerQmlTypes()
{
    qmlRegisterSingletonType<VTKController>("Instances", 1, 0, "VTKController", [](QQmlEngine *engine, QJSEngine *)->QObject *{return VTKController::instance();});
    qmlRegisterSingletonType<AppController>("Instances", 1, 0, "AppController", [](QQmlEngine *engine, QJSEngine *)->QObject *{return AppController::instance();});
    qmlRegisterSingletonType<PlaneCutController>("Instances", 1, 0, "PlaneCutController", [](QQmlEngine *engine, QJSEngine *)->QObject *{return PlaneCutController::instance();});
    qmlRegisterSingletonType<SplineCutController>("Instances", 1, 0, "SplineCutController", [](QQmlEngine *engine, QJSEngine *)->QObject *{return SplineCutController::instance();});
    qmlRegisterType<CameraParametersModel>("Models", 1, 0, "CameraParametersModel");
    qmlRegisterType<TeethModel>("Models", 1, 0, "TeethModel");

    qmlRegisterType<ViewType>("ViewType", 1, 0, "ViewType");
    qmlRegisterType<CameraMode>("CameraMode", 1, 0, "CameraMode");
}

