#include "minigame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MiniGame w;
    w.show();

    return a.exec();
}
