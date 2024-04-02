#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "Game.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    mainwindow w("saveGame.txt");
    w.show();
    return a.exec();
}