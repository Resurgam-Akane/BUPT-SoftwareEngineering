#include "coldorwarm.h"
#include "ui_workmode.h"
#include"workmode.h"
workmode::workmode(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::workmode)
{
    ui->setupUi(this);
}

workmode::~workmode()
{
    delete ui;
}

void workmode::on_coldMode_clicked()
{
    Cold_Or_Warm=0;
    //QMessageBox::information(NULL,tr("h"),tr("code mode set"));
    //controller *w=new controller();
    //w->person.WritePassword(this->Person.GetPassword());
    //w->person.WriteUsername(this->Person.GetUsername());
    //ui->hotMode->setEnabled(false);
    //w->show();
    close();
    qDebug()<<" cold mode";
}


void workmode::on_hotMode_clicked()
{
    Cold_Or_Warm=1;
    //QMessageBox::information(NULL,tr("h"),tr("hot mode set"));
    //controller *w=new controller();
    //w->person.WritePassword(this->Person.GetPassword());
    //w->person.WriteUsername(this->Person.GetUsername());
    //ui->coldMode->setEnabled(false);
    //ui->hotMode->setEnabled(false);
    //w->ui->desk->setEnabled(true);
    //w->ui->logout->setEnabled(true);
    //w->show();
    this->close();
    qDebug()<<" hot mode";
}

