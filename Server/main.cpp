#include "mainwindow.h"
#include <QApplication>

#include "connectsql.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    createConnection();
    MainWindow w;
    w.show();
    return a.exec();
}
