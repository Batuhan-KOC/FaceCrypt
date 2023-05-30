
#include "facecryptwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FaceCryptWindow w;
    w.show();
    return a.exec();
}
