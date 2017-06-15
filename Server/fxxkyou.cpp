#include "fxxkyou.h"
#include "ui_fxxkyou.h"
#include "connectsql.h"
#include"workmode.h"
fxxkyou::fxxkyou(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fxxkyou)
{
    ui->setupUi(this);
    QRegExp regExp("[1-9][0-9]{3}");
    QRegExp regExp2("[0-9]{4}");
    ui->lineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_2->setValidator(new QRegExpValidator(regExp2, this));
}

fxxkyou::~fxxkyou()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.exec("DROP TABLE airstate");
    delete ui;
}

void fxxkyou::on_pushButton_clicked()
{
    if(person.GetUsername()==""||person.GetPassword()=="")
    {
        QMessageBox::information(NULL,tr("h"),tr("Password or Username must be filled"));
    }else
    {
        if(!person.Login())
        {
            QMessageBox::information(NULL,tr("h"),tr("Please check usename or password"));
        }else
        {
            controller *w=new controller();
            w->person.WriteUsername(this->person.GetUsername());
            w->person.WritePassword(this->person.GetPassword());
            w->show();
            this->close();
            //QMessageBox::information(NULL,tr("h"),tr("Login success"));
        }
    }
}

void fxxkyou::on_lineEdit_textEdited(const QString &arg1)
{
    person.WriteUsername(arg1);
}

void fxxkyou::on_lineEdit_2_textEdited(const QString &arg1)
{
    person.WritePassword(arg1);
}

void fxxkyou::on_pushButton_2_clicked()
{
    if(person.GetUsername()==""||person.GetPassword()=="")
    {
        QMessageBox::information(NULL,tr("h"),tr("Password or Username must be filled"));
    }else
    {
        if(!person.Enroll())
        {
            QMessageBox::information(NULL,tr("h"),tr("Username have already existed"));
        }else
        {
            qDebug()<<" open controller ";
            controller *w=new controller();
            w->person.WriteUsername(this->person.GetUsername());
            w->person.WritePassword(this->person.GetPassword());
            w->show();
            qDebug()<<w->person.GetUsername()<<"     username";
            qDebug()<<w->person.GetPassword()<<"      passowrd";
            this->close();
            //QMessageBox::information(NULL,tr("h"),tr("Enroll success"));
        }
    }
}

void fxxkyou::on_pushButton_3_clicked()
{
    workmode *w=new workmode();
    w->show();
    //ui->pushButton_3->setEnabled(false);
}

void fxxkyou::closeEvent(QCloseEvent *event){
    //QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    //QSqlQuery query(db);
    //if(query.exec("DROP TABLE airstate"));
}

