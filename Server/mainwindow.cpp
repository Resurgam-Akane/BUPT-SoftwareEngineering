#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QFile>


int computerHour;
int computerMinute;
int computerSecond;
int computerWeek;
int computerDay;
int computerMonth;
int computerYear;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server.listen(QHostAddress::Any,6666);
    widget = new statementshow(this);
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
    //add by wang 2017/6/4
    //读取自定义的日期
    /*QFile file("time.txt");

    if(file.open(QFile::ReadOnly | QFile::Text)){
        QString read;
        QTextStream in(&file);
        in>>read;
        computerYear = read.toInt();
        in>>read;
        computerMonth = read.toInt();
        in>>read;
        computerDay = read.toInt();
        in>>read;
        computerHour = read.toInt();
        in>>read;
        computerMinute = read.toInt();
        in>>read;
        computerSecond = read.toInt();
        in>>read;
        computerWeek = read.toInt();
    }
    qDebug()<<computerYear<<computerMonth<<computerDay;*/

    connect(widget,&statementshow::ifPush,this,[&](){
        this->ui->pushButton->setEnabled(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->ui->pushButton->setEnabled(false);
    widget->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("DELETE FROM roomfee WHERE newesttime = ?");
    query.bindValue(0, 1);
    query.exec();
}
