#include"adminstrator.h"
Adminstrator::Adminstrator()
{
    this->Frequency=2000;
    this->Username="";
    this->Password="";
}

void Adminstrator::WriteUsername(QString username)
{
    this->Username=username;
}

void Adminstrator::WritePassword(QString password)
{
    this->Password=password;
}

QString Adminstrator::GetPassword()
{
    return this->Password;
}

QString Adminstrator::GetUsername()
{
    return this->Username;
}

bool Adminstrator::Enroll()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.exec("select * from adminstrator");
    while(query.next())
    {
        if(query.value(0).toString()==this->GetUsername())
        {
            return false;
        }
    }
    qDebug()<<" insert into database";
    query.exec();
    query.prepare("insert into adminstrator(Username,Password,Status)"
                  "values (:Username, :Password, :Status)");
    query.bindValue(0, this->GetUsername());
    query.bindValue(1, this->GetPassword());
    query.bindValue(2, 1);
    query.exec();
    return true;
}

bool Adminstrator::Login()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.exec("select * from adminstrator");
    while(query.next())
    {
        if(query.value(0).toString()==GetUsername()/*&&query.value(2).toInt()==0*/)
        {
            if(query.value(1).toString()== GetPassword())
            {
                query.prepare("Update adminstrator set Status = ? where Username= ? and Password = ?;");
                query.addBindValue(1);
                query.addBindValue(GetUsername());
                query.addBindValue(GetPassword());
                query.exec();
               return true;
            }else
            {
               query.exec();
               return false;
            }
        }
    }
    query.exec();
    return false;
}

bool Adminstrator::LogOut()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.exec("select * from adminstrator");
    while(query.next())
    {
        qDebug()<<query.value(0).toString()<<" username "<<" ";
        if(query.value(0).toString()==this->GetUsername()/*&&query.value(2).toInt()==1*/)
        {
            if(query.value(1).toString()==this->GetPassword())
            {
                query.exec();
                /*query.prepare("Update adminstrator set Status = ? where Username= ? and Password = ?;");
                query.addBindValue(0, 0);
                query.addBindValue(1, this->GetUsername());
                query.addBindValue(2, this->GetPassword());
                query.exec();*/
               return true;
            }else
            {
               query.exec();
               return false;
            }
        }
    }
    qDebug()<<" out of while ";
    query.exec();
    return false;
}
