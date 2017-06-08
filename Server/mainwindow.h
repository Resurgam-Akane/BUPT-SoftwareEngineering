#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"
#include "tcpsocket.h"
#include "threadhandle.h"
#include "globalvar.h"

#include "statementshow.h"
#include <QDebug>
#include <QTextStream>
#include "globalvar.h"
#include "connectsql.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    TcpServer server;

    statementshow * widget;

protected:
     void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
