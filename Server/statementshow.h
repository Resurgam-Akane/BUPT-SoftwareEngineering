#ifndef STATEMENTSHOW_H
#define STATEMENTSHOW_H

#include <QMainWindow>
#include <QDebug>
#include "connectsql.h"

namespace Ui {
class statementshow;
}

class statementshow : public QMainWindow
{
    Q_OBJECT

public:
    explicit statementshow(QWidget *parent = 0);
    ~statementshow();

private slots:
    void on_pushButtonAll_clicked();

    void on_pushButtonDay_clicked();

    void on_pushButtonWeek_clicked();

    void on_pushButtonMonth_clicked();

    void on_pushButtonOne_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

private:
    Ui::statementshow *ui;
    QString viewnumber;

protected:
     void closeEvent(QCloseEvent *event);

signals:
    void ifPush();
};

#endif // STATEMENTSHOW_H
