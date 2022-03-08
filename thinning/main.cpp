#include "thinningmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ThinningMainWindow w;
    w.show();
    return a.exec();
}
