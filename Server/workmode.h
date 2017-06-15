#ifndef WORKMODE_H
#define WORKMODE_H
#include <QMessageBox>
#include <QMainWindow>
#include"globalvar.h"
#include"adminstrator.h"
#include"controller.h"
namespace Ui {
class workmode;
}

class workmode : public QMainWindow
{
    Q_OBJECT

public:
    explicit workmode(QWidget *parent = 0);
    ~workmode();
    Adminstrator Person;

private slots:
    void on_coldMode_clicked();

    void on_hotMode_clicked();

private:
    Ui::workmode *ui;
};

#endif // WORKMODE_H
