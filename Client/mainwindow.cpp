#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    realTimeRoomTemperature(25),
    windVelocity(0),
    timerRealTime(new QTimer(this)),
    timerWoking(new QTimer(this))
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    this->ui->UpTemperatureBtn->setEnabled(false);
    this->ui->DownTemperatureBtn->setEnabled(false);
    this->ui->HighVelocityBtn->setEnabled(false);
    this->ui->MidVelocityBtn->setEnabled(false);
    this->ui->LowVelocityBtn->setEnabled(false);
    this->ui->RealTimeTemperatureLabel->setText("25 Celsius");
    this->ui->TargetTemperatureLabel->setText("");
    QRegExp regExp("[0-9]+");
    ui->UserIDLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->RoomNumLineEdit->setValidator(new QRegExpValidator(regExp, this));
    //add by xuzhu
    connect(modifyTemperature.tm, &QTimer::timeout, this, [&](){
        modifyTemperature.tm->stop();
        targetTemperature = this->ui->TargetTemperatureLabel->text().toFloat();
        if ((realTimeRoomTemperature == targetTemperature) || (realTimeRoomTemperature < targetTemperature && workMode == 0) || (realTimeRoomTemperature > targetTemperature && workMode == 1)) {
            QString data = "requestEnd";
            tcpClient->write(data.toUtf8());
            qDebug() << "发送数据" << data;
            timerRealTime->start(10000);
            timerWoking->stop();
            this->ui->StatusLabel->setText("Standby");
        }
        else {
            QString data = "request/" + QString("%1").arg(realTimeRoomTemperature) + "/" + this->ui->TargetTemperatureLabel->text();
            tcpClient->write(data.toUtf8());
            qDebug() << "发送数据" << data;
        }
    });

    connect(timerRealTime, &QTimer::timeout, this, [&](){
        //qDebug() << targetTemperature << " " << realTimeRoomTemperature;

        if(realTimeRoomTemperature < 25)
        {
            qDebug() << "every 10s, realTimeRoomTemperature change";
            ++realTimeRoomTemperature;
            this->ui->RealTimeTemperatureLabel->setText(QString("%1 Celsius").arg(realTimeRoomTemperature));
        }
        else if (realTimeRoomTemperature > 25)
        {
            qDebug() << "every 10s, realTimeRoomTemperature change";
            --realTimeRoomTemperature;
            this->ui->RealTimeTemperatureLabel->setText(QString("%1 Celsius").arg(realTimeRoomTemperature));
        }

        if(this->ui->StatusLabel->text() == "Standby" && targetTemperature < realTimeRoomTemperature && workMode == 0){
            QString data = "request/" + QString("%1").arg(realTimeRoomTemperature) + "/" + QString("%1").arg(targetTemperature);
            tcpClient->write(data.toUtf8());
            qDebug() << "发送数据:" << data;
        }
        else if (this->ui->StatusLabel->text() == "Standby" && targetTemperature > realTimeRoomTemperature && workMode == 1) {
            QString data = "request/" + QString("%1").arg(realTimeRoomTemperature) + "/" + QString("%1").arg(targetTemperature);
            tcpClient->write(data.toUtf8());
            qDebug() << "发送数据:" << data;
        }
    });
    timerRealTime->start(10000);

    connect(timerWoking, &QTimer::timeout, this, [&](){
        if (this->ui->StatusLabel->text() == "Working") {
        if (realTimeRoomTemperature < targetTemperature)
        {
            qDebug() << "RoomTemperature change";
            ++realTimeRoomTemperature;
            this->ui->RealTimeTemperatureLabel->setText(QString("%1 Celsius").arg(realTimeRoomTemperature));
        }
        else if (realTimeRoomTemperature > targetTemperature)
        {
            qDebug() << "RoomTemperature change";
            --realTimeRoomTemperature;
            this->ui->RealTimeTemperatureLabel->setText(QString("%1 Celsius").arg(realTimeRoomTemperature));
        }

        if (realTimeRoomTemperature == targetTemperature) {
            QString data = "requestEnd";
            tcpClient->write(data.toUtf8());
            qDebug() << "发送数据:" << data;
            timerRealTime->start(10000);
            timerWoking->stop();
            this->ui->StatusLabel->setText("Standby");
        }
        }
    });

    connect(tcpClient,&QTcpSocket::readyRead,[&](){
        QString data = QString(this->tcpClient->readAll());
        QString handleData;
        qDebug() << "收到数据" << data;
        while(!data.isEmpty()) {
            if (data.left(9) == "serverMsg")
            {
                if (data.left(16) == "serverMsg/reject") {
                    handleData = data.mid(0,16);
                    data = data.mid(16);
                    QMessageBox::information(this, tr("Fail"),
                                             tr("Your login is rejected. Please check your "
                                                "information is right."));
                    tcpClient->disconnectFromHost();
                    if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000) )
                    {
                        this->ui->LoginBtn->setText("Login");
                        qDebug() << "断开服务器";
                        //将其他按钮和输入栏设置为false
                        this->ui->UserIDLineEdit->setEnabled(true);
                        this->ui->RoomNumLineEdit->setEnabled(true);
                        this->ui->ConsumptionLabel->setText("0");
                        this->ui->FeeLabel->setText("0");
                    }
                }
                else {
                    QString serverMsgInfoPattern = ("serverMsg/([0-1])/([0-9]*)/([0-9]*)");
                    QRegExp rx(serverMsgInfoPattern);
                    handleData = data.mid(0,17);
                    data = data.mid(17);

                    int pos = handleData.indexOf(rx);
                    qDebug() << pos;
                    if (pos < 0) return;

                    this->ui->StatusLabel->setText("Standby");
                    qDebug() << rx.capturedTexts();
                    workMode = rx.cap(1).toInt();
                    workMode ? this->ui->WorkModeLabel->setText("heat") : this->ui->WorkModeLabel->setText("cool");
                    targetTemperature = workMode ? 28 : 22;
                    this->ui->TargetTemperatureLabel->setText(QString("%1").arg(targetTemperature));
                    lowerLimit = rx.cap(2).toFloat();
                    this->ui->LowestTemperatureLabel->setText(rx.cap(2));
                    upperLimit = rx.cap(3).toFloat();
                    this->ui->HighestTemperatureLabel->setText(rx.cap(3));

                    if (realTimeRoomTemperature != targetTemperature) {
                        QString reData = "request/" + QString("%1").arg(realTimeRoomTemperature) + "/" + QString("%1").arg(targetTemperature);
                        tcpClient->write(reData.toUtf8());
                        qDebug() << "发送数据:" << reData;
                    }
                }
            }
            else if (data.left(6) == "answer")
            {
                handleData = data.mid(0, 6);
                data = data.mid(6);
                if (this->ui->StatusLabel->text() != "Working") {
                    this->ui->StatusLabel->setText("Working");
                    timerRealTime->stop();
                    timerWoking->start(5000);
                }
                //todo::
            }
            else if (data.left(10) == "acceptWind")
            {
                handleData = data.mid(0,10);
                data = data.mid(10);
                if (windVelocity == 0)
                    this->ui->WindVelocityLabel->setText("Low");
                else if (windVelocity == 1)
                    this->ui->WindVelocityLabel->setText("Mid");
                else this->ui->WindVelocityLabel->setText("High");
            }
            else if (data.left(7) == "request")
            {
            //answer／当前温度／目标温度／风速
                handleData = data.mid(0,7);
                data = data.mid(7);
                QString reData = "answer/" + QString("%1").arg(realTimeRoomTemperature) + "/" + QString("%1").arg(targetTemperature) + "/" + QString("%1").arg(windVelocity);
                tcpClient->write(reData.toUtf8());
                qDebug() << "发送数据:" << reData;
            }
            else if (data.left(4) == "cost")
            {
                QString costInfoPattern = ("cost\/([0-9]+(\.[0-9]+)?)\/([0-9]+(\.[0-9]+)?)");
                QRegExp rx(costInfoPattern);
                int pos = data.indexOf(rx);
                qDebug() << pos;
                qDebug() << rx.capturedTexts();
                if (pos < 0) return;
                data = rx.cap(5);

                this->ui->ConsumptionLabel->setText(QString("%1").arg(rx.cap(1).toFloat()));
                this->ui->FeeLabel->setText(QString("%1").arg(rx.cap(3).toFloat()));
            }
        }
    });
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(tcpClient, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, &MainWindow::displayError);
    connect(tcpClient,&QTcpSocket::disconnected,[&](){
        //由于服务器关闭导致的disconnect时，client所需要的操作都在这里写
        qDebug()<< "disconnect" ;
        if (tcpClient->state() == QAbstractSocket::UnconnectedState)
        {
            this->ui->LoginBtn->setText("Login");
            //qDebug() << "断开服务器";
            //将其他按钮和输入栏设置为false
            this->ui->UserIDLineEdit->setEnabled(true);
            this->ui->RoomNumLineEdit->setEnabled(true);
            this->ui->StatusLabel->setText("ShutDown");
            this->ui->UpTemperatureBtn->setEnabled(false);
            this->ui->DownTemperatureBtn->setEnabled(false);
            this->ui->HighVelocityBtn->setEnabled(false);
            this->ui->MidVelocityBtn->setEnabled(false);
            this->ui->LowVelocityBtn->setEnabled(false);
            this->ui->WorkModeLabel->setText("");
            this->ui->HighestTemperatureLabel->setText("");
            this->ui->LowestTemperatureLabel->setText("");
            this->ui->WindVelocityLabel->setText("");
            windVelocity = 0;
            timerRealTime->start(10000);
        }
    });

    connect(tcpClient,&QTcpSocket::connected, [&](){
        this->ui->LoginBtn->setText("Out");
        qDebug() << "连接服务器成功";
        //将其他按钮和输入栏设置为true
        this->ui->UserIDLineEdit->setEnabled(false);
        this->ui->RoomNumLineEdit->setEnabled(false);
        this->ui->UpTemperatureBtn->setEnabled(true);
        this->ui->DownTemperatureBtn->setEnabled(true);
        this->ui->HighVelocityBtn->setEnabled(true);
        this->ui->MidVelocityBtn->setEnabled(true);
        this->ui->LowVelocityBtn->setEnabled(true);
        this->ui->ConsumptionLabel->setText("0");
        this->ui->FeeLabel->setText("0");
        this->ui->WindVelocityLabel->setText("Low");
        qDebug() << "发送RoomNum和UserID" ;

        //“clientMsg/房间号/身份证号”
        QString data = "clientMsg/" + RoomNum + "/" + UserID;
        if (data.isEmpty())
        {
            return ;
        }
        tcpClient->write(data.toUtf8());
        qDebug() << "发送数据:" << data;

/*        if (realTimeRoomTemperature != targetTemperature) {
            data = "request";
            tcpClient->write(data.toUtf8());
            qDebug() << "发送数据:" << data;
        }   */
    });

}

MainWindow::~MainWindow()
{
    delete tcpClient;
    delete ui;
}

void MainWindow::on_LoginBtn_clicked()
{
    qDebug() << "点击登陆";

    //this->ui->TargetTemperatureLabel->setText("25");
    if ("Login" == this->ui->LoginBtn->text())
    {
        this->UserID = this->ui->UserIDLineEdit->text();
        this->RoomNum = this->ui->RoomNumLineEdit->text();
        if (UserID.size() <= 4 && RoomNum.size() <= 4)
        {
            bool isRight = true;
            for(auto i : this->UserID) {
                if (!(i <= '9' && i >= '0')) isRight = false;
            }

            for(auto i : this->RoomNum) {
                if (!(i <= '9' && i >= '0')) isRight = false;
            }

            if (isRight)
                tcpClient->connectToHost("127.0.0.1",6666);
            else
                QMessageBox::information(this, tr("Input wrong"),
                            tr("Please inspect your input"
                               "length must be less than 4,"
                               "input must consist of number."));

        }
        else
        {
            QMessageBox::information(this, tr("Input wrong"),
                        tr("Please inspect your input"
                           "length must be less than 4,"
                           "input must consist of number."));
            return;
        }
    }
    else
    {
        //client断开连接时，client所需要的操作都在这里写
        tcpClient->disconnectFromHost();
        if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000) )
        {
            this->ui->LoginBtn->setText("Login");
            qDebug() << "断开服务器";
            //将其他按钮和输入栏设置为false
            this->ui->UserIDLineEdit->setEnabled(true);
            this->ui->RoomNumLineEdit->setEnabled(true);
            this->ui->ConsumptionLabel->setText("0");
            this->ui->FeeLabel->setText("0");
        }
    }
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("AirConditioner Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("AirConditioner Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("AirConditioner Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpClient->errorString()));
    }
}

void MainWindow::on_HighVelocityBtn_clicked()
{
    QString data = "wind/2";
    windVelocity = 2;
    if (data.isEmpty())
    {
        return ;
    }
    tcpClient->write(data.toUtf8());
    qDebug() << "发送数据:" << data;
}

void MainWindow::on_MidVelocityBtn_clicked()
{
    QString data = "wind/1";
    windVelocity = 1;
    if (data.isEmpty())
    {
        return ;
    }
    tcpClient->write(data.toUtf8());
    qDebug() << "发送数据:" << data;
}

void MainWindow::on_LowVelocityBtn_clicked()
{
    QString data = "wind/0";
    windVelocity = 0;
    if (data.isEmpty())
    {
        return ;
    }
    tcpClient->write(data.toUtf8());
    qDebug() << "发送数据:" << data;
}

void MainWindow::on_UpTemperatureBtn_clicked()
{
    QString t = this->ui->TargetTemperatureLabel->text();
    if (t.toFloat()<upperLimit)
        t = QString("%1").arg(t.toFloat()+1);
    modifyTemperature.up();
    this->ui->TargetTemperatureLabel->setText(t);
}

void ModifyTargetTemperature::up()
{
    tm->start(1000);
}

void ModifyTargetTemperature::down()
{
    tm->start(1000);
}

void MainWindow::on_DownTemperatureBtn_clicked()
{
    QString t = this->ui->TargetTemperatureLabel->text();
    if (t.toFloat()>lowerLimit)
        t = QString("%1").arg(t.toFloat()-1);
    modifyTemperature.down();
    this->ui->TargetTemperatureLabel->setText(t);
}
