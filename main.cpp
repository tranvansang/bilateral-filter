#include "bilateralfiltering.h"
#include <QApplication>

#include <QSurfaceFormat>>

int main(int argc, char *argv[])
{
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    surfaceFormat.setMajorVersion(4);
    surfaceFormat.setMajorVersion(5);
    QSurfaceFormat::setDefaultFormat(surfaceFormat);

    //qDebug() << QSurfaceFormat::defaultFormat();
    QApplication a(argc, argv);
    BilateralFiltering w;
    w.show();

    return a.exec();
}
