#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
    MainWindow w;
    w.show();
    return a.exec();
}
