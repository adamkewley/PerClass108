#include "mainwindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a{argc, argv};
    pc::MainWindow w;
    w.show();
    return a.exec();
}
