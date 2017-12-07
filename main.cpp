#include "circleclock.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CircleClock w;
    w.show();

    return a.exec();
}
