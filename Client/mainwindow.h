#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>
#include <QString>
#include <QTimer>

namespace Ui {
class MainWindow;
class ModifyTargetTemperature;
}

class ModifyTargetTemperature : public QObject
{
    Q_OBJECT
public:
    ModifyTargetTemperature(){tm = new QTimer(this);}
    void up();
    void down();
    QTimer *tm;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void displayError(QAbstractSocket::SocketError socketError);


private slots:
    void on_LoginBtn_clicked();

    void on_HighVelocityBtn_clicked();

    void on_MidVelocityBtn_clicked();

    void on_LowVelocityBtn_clicked();

    void on_UpTemperatureBtn_clicked();

    void on_DownTemperatureBtn_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket * tcpClient;
    QString UserID;
    QString RoomNum;
    float realTimeRoomTemperature;
    float targetTemperature;
    int windVelocity;
    ModifyTargetTemperature modifyTemperature;
    QTimer *timerRealTime;
    QTimer *timerWoking;

    int workMode;
    float upperLimit;
    float lowerLimit;
    //QMap<float, QDateTime> modifyTargetTemperature;
};

#endif // MAINWINDOW_H
