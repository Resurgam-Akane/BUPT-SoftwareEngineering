#include "statementshow.h"
#include <QRegExp>
#include "ui_statementshow.h"
#include"controller.h"
statementshow::statementshow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::statementshow)
{
    ui->setupUi(this);
    QRegExp regExp("[0-9]{4}");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp, this));
}

statementshow::~statementshow()
{
    delete ui;    
}

void statementshow::on_pushButtonAll_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select roomnum,onofftimes,starttime, endtime, starttemperature, endtemperature,"
                    "startwind, endwind, fee from roomfee where newesttime = 0");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("roomnum"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("onofftimes"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("start:m-d-h-m"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("end:m-d-h-m"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("starttemperature"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("endtemperature"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("startwind"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("endwind"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("fee"));
    //model->setHeaderData(9, Qt::Horizontal, QObject::tr("newesttime"));
    ui->tableView->setModel(model);
}

void statementshow::on_pushButtonDay_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select roomnum,month,day,SUM(expense) from roomadjust group by roomnum,day");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("roomnum"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("month"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("day"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("expense"));
    ui->tableView_2->setModel(model);
}


void statementshow::on_pushButtonWeek_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select roomnum,week,SUM(expense) from roomadjust group by roomnum,week");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("roomnum"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("week"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("expense"));
    ui->tableView_2->setModel(model);
}

void statementshow::on_pushButtonMonth_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select roomnum,month,SUM(expense) from roomadjust group by roomnum,month");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("roomnum"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("month"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("expense"));
    ui->tableView_2->setModel(model);
}

void statementshow::on_pushButtonOne_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("select roomnum,onofftimes,starttime, endtime, starttemperature, endtemperature,"
                    "startwind, endwind, fee from roomfee where roomnum = ? and newesttime = 0");
    query.addBindValue(viewnumber);
    query.exec();
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("roomnum"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("onofftimes"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("start:m-d-h-m"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("end:m-d-h-m"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("starttemperature"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("endtemperature"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("startwind"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("endwind"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("fee"));
    //model->setHeaderData(9, Qt::Horizontal, QObject::tr("newesttime"));
    ui->tableView->setModel(model);
}

void statementshow::on_lineEdit_textEdited(const QString &arg1)
{
    viewnumber = arg1;
}

void statementshow::closeEvent(QCloseEvent *event)
{
    emit ifPush();
}
