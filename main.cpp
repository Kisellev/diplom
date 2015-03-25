#include "mainwindow.h"
#include <QApplication>

#include "sett.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Проект");
    w.show();

    return a.exec();
}

