#include "screen_off.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    screen_off w;
    w.show();
    return a.exec();
}
