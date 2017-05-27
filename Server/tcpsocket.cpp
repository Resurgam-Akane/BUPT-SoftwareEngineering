#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>
#include <QRegExp>

TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    isFirstRequest = true;
    windVelocity = "0";
    fee = 0;
    consumption = 0;
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readData);

    //todo:此处的频率可以设定，使用全局变量，放在一个名字空间里
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&](){
       this->write("request");
    });
    timer->start(2000);

    //add by xuzhu 2017/5/27
    timerCost = new QTimer(this);
    connect(timerCost, &QTimer::timeout, this, [&](){
        QDateTime curTime = QDateTime::currentDateTime();
        int workSec = timeStart.secsTo(curTime);

        //计算
        if (windVelocity == "0") {
            consumption = workSec;
            fee = consumption;
        }
        else if (windVelocity == "1") {
            consumption = workSec * 2;
            fee = consumption;
        }
        else {
            consumption = workSec * 3;
            fee = consumption;
        }
        QString data = "cost/" + QString("%1").arg(consumption) + QString("%1").arg(fee);
        this->write(data.toUtf8());
    });


    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            qDebug() << "disconnect";
            timer->stop();
            timerCost->stop();

            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    qDebug() << "new connect" ;
}

TcpSocket::~TcpSocket()
{
    delete timerCost;
    timerCost = NULL;
}


void TcpSocket::sentData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
}

void TcpSocket::disConTcp(int i)
{
    if (i == socketID)
    {
        this->disconnectFromHost();
    }
    else if (i == -1) //-1为全部断开
    {
        disconnect(dis); //先断开连接的信号槽，防止二次析构
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::readData()
{
//    datas.append(this->readAll());
    auto data  = handleData(this->readAll(),this->peerAddress().toString(),this->peerPort());
    qDebug() << "发送数据:" << data;
    this->write(data);
//    if (!watcher.isRunning())//放到异步线程中处理。
//    {
//        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
//    }
}

QByteArray TcpSocket::handleData(QByteArray data, const QString &ip, qint16 port)
{
    //数据处理放在这里
    QTime time;
    time.start();

    QElapsedTimer tm;
    tm.start();
    while(tm.elapsed() < 100)
    {}
    QByteArray reData, handleData;
    //data = ip.toUtf8() + " " + QByteArray::number(port) + " " + data + " " + QTime::currentTime().toString().toUtf8();
    //serverMsg/工作模式/最低可设置温度/最高可设置温度
    //目前假设数据。
    //reData为回复的字符流，data为收到的字符流，handledata为处理的字符流
    while (!data.isEmpty()) {
        qDebug() << "收到数据:" << data;
        if (data.left(9) == "clientMsg")
        {
            //todo:
            handleData = data.mid(0, 19);
            data = data.mid(19);
            roomNum = handleData.mid(10, 4);
            userID = handleData.mid(15);
            reData = "serverMsg/0/18/25";
        }
        else if (data.left(10) == "requestEnd")
        {
            handleData = data.mid(0, 10);
            data = data.mid(10);
            timerCost->stop();
            timeEnd = QDateTime::currentDateTime();
            QString cur = QDateTime::currentDateTime().toString(Qt::ISODate);
            int workSec = timeStart.secsTo(timeEnd);
            isFirstRequest = true;

            //计算
            if (windVelocity == "0") {
                consumption = workSec;
                fee = consumption;
            }
            else if (windVelocity == "1") {
                consumption = workSec * 2;
                fee = consumption;
            }
            else {
                consumption = workSec * 3;
                fee = consumption;
            }
            //QString data = "cost/" + QString("%1").arg(consumption) + QString("%2").arg(fee);
            QString con = QString("%1").arg(consumption);
            QString fe = QString("%2").arg(fee);

            reData = "cost/" + con.toUtf8() + "/" + fe.toUtf8();
        }
        else if (data.left(4) == "wind")
        {
            handleData = data.mid(0,6);
            data = data.mid(6);
            //qDebug() << handleData;
            windVelocity = handleData[handleData.length()-1];
            qDebug() << windVelocity;
            //负载允许的条件下，发送acceptWind
            reData = "acceptWind";
        }
        else if (data.left(7) == "request")
        {
            handleData = data.mid(0,7);
            data = data.mid(7);
            if (isFirstRequest) {
                isFirstRequest = false;
                timerCost->start(60000);
                timeStart = QDateTime::currentDateTime();
                QString cur = QDateTime::currentDateTime().toString(Qt::ISODate); //第一次request的时间
                //将第一次request的时间写入数据库 记得加上新的目标温度
            }
            else {
                QString cur = QDateTime::currentDateTime().toString(Qt::ISODate); //第二、三等次request的时间
                //将第二、三次request的时间写入数据库 记得加上新的目标温度
            }

            //todo:在负载允许的条件下，返回answer。 记录下回anwser的时间
            reData = "answer";
        }
        else if (data.left(6) == "answer")
        {
            handleData = data.mid(0,14);
            data = data.mid(14);

            QString words(data);
            QString answerPattern("answer/([0-9]*)/([0-9]*)/(0|1|2)");
            QRegExp rx(answerPattern);

            int pos = words.indexOf(rx);
            if (pos < 0) return "";

            qDebug() << rx.capturedTexts(); //("answer/25/22/0", "25", "22", "0")
            reData = "";
        }
    }
    return reData;
}

void TcpSocket::startNext()
{
    this->write(watcher.future().result());
    if (!datas.isEmpty())
    {
        watcher.setFuture(QtConcurrent::run(this,&TcpSocket::handleData,datas.dequeue(),this->peerAddress().toString(),this->peerPort()));
    }
}
