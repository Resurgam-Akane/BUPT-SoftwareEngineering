#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMainWindow>
#include <QCloseEvent>
#include"adminstrator.h"
#include"workmode.h"
#include"fxxkyou.h"
//#include"signin.h"
#include "tcpserver.h"
#include "tcpsocket.h"
#include "threadhandle.h"
#include "globalvar.h"

#include "statementshow.h"
#include <QDebug>
#include <QTextStream>
#include "connectsql.h"
#include <QTimer>
namespace Ui {
class controller;
}

class controller : public QMainWindow
{
    Q_OBJECT

public:
    explicit controller(QWidget *parent = 0);
    ~controller();
    Adminstrator person;
private slots:
    void on_desk_clicked();

    void on_logout_clicked();

    //void on_workmode_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_frequency_clicked();

    void on_pushButton_clicked();

private:
    Ui::controller *ui;
    TcpServer server;
    statementshow * widget;
    QTimer *refreshTimer;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CONTROLLER_H
