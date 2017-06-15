#include <QApplication>
#include "connectsql.h"
#include"fxxkyou.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    createConnection();
    //signIn w;
    //w.show();
     fxxkyou w;
     w.show();
    return a.exec();
}
