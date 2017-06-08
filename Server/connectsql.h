#ifndef CONNECTSQL_H
#define CONNECTSQL_H

#include <QTime>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRelation>
#include <QSqlError>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("statement.db");
    if (!db.open()) {
        QMessageBox::critical(0, "Cannot open the statement",
            "Unable to establish a statement connection.", QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query(db);
    query.exec("create table roomfee(roomnum String key,"
               "onofftimes int, starttime String, endtime String, starttemperature String, endtemperature String,"
               "startwind String, endwind String, fee int, newesttime int)");
    query.exec("create table roomadjust(roomnum String key,"
               "date String, expense float, month int, day int, hour int, min int, week int)");
    return true;
}


#endif // CONNECTSQL_H
