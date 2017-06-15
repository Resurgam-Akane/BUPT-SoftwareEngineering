#ifndef FXXKYOU_H
#define FXXKYOU_H

#include <QMainWindow>
#include"adminstrator.h"
#include"controller.h"
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
#include <QRegExp>
namespace Ui {
class fxxkyou;
}

class fxxkyou : public QMainWindow
{
    Q_OBJECT

public:
    explicit fxxkyou(QWidget *parent = 0);
    ~fxxkyou();
    Adminstrator person;
private slots:
    void on_pushButton_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::fxxkyou *ui;
};

#endif // FXXKYOU_H
