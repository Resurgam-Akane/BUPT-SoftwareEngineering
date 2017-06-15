#include "controller.h"
#include "ui_controller.h"
#include <QTextStream>
#include <QFile>
#include"coldorwarm.h"
controller::controller(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::controller)
{
    ui->setupUi(this);
    //server = new TcpServer(this);
    QRegExp regExp("[1-9]|10");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp, this));
    server.listen(QHostAddress::Any,6666);
    widget = new statementshow(this);
    refreshTimer = new QTimer(this);
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
    connect(widget,&statementshow::ifPush,this,[&](){
        this->ui->desk->setEnabled(true);
    });
    refreshTimer->start(500);  //默认刷新频率是1秒2次
    connect(refreshTimer, &QTimer::timeout, [&](){
        QSqlDatabase db = QSqlDatabase::database("QSQLITE");
        QSqlQuery query(db);
        QSqlQueryModel *model = new QSqlQueryModel(this);
        model->setQuery("select * from airstate");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Room Num"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("User ID"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Room Temp"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Target Temp"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Wind Vecolity"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Running State"));
        ui->tableView->setModel(model);
    });
}

controller::~controller()
{
    delete ui;
}

void controller::on_desk_clicked()
{
    this->ui->desk->setEnabled(false);
    //this->ui->logout->setEnabled(false);
    //this->ui->workmode->setEnabled(false);
    widget->show();
}

void controller::on_logout_clicked()
{
    server.sentDisConnect(-1);
    qDebug()<<person.GetUsername()<<"     username";
    qDebug()<<person.GetPassword()<<"      passowrd";
    if(person.LogOut())
    {
       fxxkyou *w=new fxxkyou();
       w->show();
       close();
      // QMessageBox::information(NULL,tr("h"),tr("Logout success"));
    }else if(!person.LogOut())
    {
       QMessageBox::information(NULL,tr("h"),tr("Logout fail"));
    }
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    if(query.exec("DROP TABLE airstate"));
}

/*void controller::on_workmode_clicked()
{
    workmode *w=new workmode();
    w->show();
    //this->ui->desk->setEnabled(false);
    //this->ui->logout->setEnabled(false);
    ui->workmode->setEnabled(false);
}*/

void controller::closeEvent(QCloseEvent *event)
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("DELETE FROM roomfee WHERE newesttime = ?");
    query.bindValue(0, 1);
    query.exec();
    query.clear();
    if(query.exec("DROP TABLE airstate"));
}

void controller::on_lineEdit_textEdited(const QString &arg1)//设置刷新频率
{
    if(arg1=="")
    {
      person.Frequency=0;
    }else
    {
      person.Frequency=arg1.toInt()*1000;
    }
}

void controller::on_frequency_clicked()
{
    if(person.Frequency<=0)
     {
        QMessageBox::information(NULL,tr("h"),tr("Invalid Input"));
     }else
     {
        //ui->frequency->setEnabled(false);
        //ui->lineEdit->setEnabled(false);
        frequency=person.Frequency;
     }
    //qDebug()<<Cold_Or_Warm<<" Cold_Or_Warm";
    qDebug()<<frequency<<" frequency";
    refreshTimer->start(frequency);
}

void controller::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from airstate");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Room Num"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("User ID"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Room Temp"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Target Temp"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Wind Vecolity"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Running State"));
    ui->tableView->setModel(model);
}

