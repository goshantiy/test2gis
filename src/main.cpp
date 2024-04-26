#include <QGuiApplication>
#include "app.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    TestApp testApp;

    return app.exec();
}
