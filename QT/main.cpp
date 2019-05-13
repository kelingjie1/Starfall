#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        format.setVersion(3,3);
        format.setProfile(QSurfaceFormat::CompatibilityProfile);
    }
    else
    {
        format.setVersion(3,0);
    }
    QSurfaceFormat::setDefaultFormat(format);
//    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
