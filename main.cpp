#include "bilateralfiltering.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BilateralFiltering w;
    w.show();

    return a.exec();
}
