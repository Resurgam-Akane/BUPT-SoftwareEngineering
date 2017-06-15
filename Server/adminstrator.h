#ifndef ADMINSTRATOR_H
#define ADMINSTRATOR_H
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QString>
#include <QSqlTableModel>
#include <QSqlRelation>
#include <QSqlError>
#include <QDebug>
class Adminstrator
{
   public:
    Adminstrator();
    void WriteUsername(QString username);
    void WritePassword(QString password);
    QString GetUsername();
    QString GetPassword();
    int Frequency;
    bool Enroll();//验证是否注册成功
    bool Login();//验证是否登入成功
    bool LogOut();//验证是否退出成功
   private:
    QString  Username;
    QString  Password;
};
#endif // ADMINSTRATOR_H
