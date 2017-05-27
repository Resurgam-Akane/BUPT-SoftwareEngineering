#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server.listen(QHostAddress::Any,6666);
}

MainWindow::~MainWindow()
{
    delete ui;
}
