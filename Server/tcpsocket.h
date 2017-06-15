#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QQueue>
#include <QFutureWatcher>
#include <QByteArray>
#include <QTime>
//add by xuzhu 2017/5/27
#include <QTimer>
#include <QDateTime>
#include <tuple>

#include "connectsql.h"

#define FACTOR 24


class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(qintptr socketDescriptor, QObject *parent = 0);
    ~TcpSocket();
    QByteArray handleData(QByteArray data,const QString & ip, qint16 port);//用来处理数据的函数

//add by wang 2017/6/4
    //用于报表处理

    void sqlonoffTimes();
    void sqlstartend();
    void sqlstartWindchange();
    void sqlstartTempchange(QString Ctemp);
    void sqlendWindchange();
    void sqlendTempchange(QString Ctemp);
    void sqlnewRequire(QString Ctemp1, QString Ctemp2);
    void sqlsetwind();
    void sqlsetwaitstate();
    void sqlsetanwstate(QString Ctemp1, QString Ctemp2);
    void sqlsetrunstate();
    void sqlsetcoldstate();
    void sqlsethotstate();
    QString mimicTime();

signals:
    //void readData(const int,const QString &,const quint16,const QByteArray &);
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
    void reFresh();
public slots:
    void sentData(const QByteArray & ,const int);//发送信号的槽
    void disConTcp(int i);
    void sentAnswer(TcpSocket*);

protected slots:
    void readData();//接收数据
    void startNext();//处理下一个
protected:
    QFutureWatcher<QByteArray> watcher;
    QQueue<QByteArray> datas;
private:
    qintptr socketID;
    QMetaObject::Connection dis;
    QTimer *timer;
    QTimer *timerCost;
    QDateTime timeStart;
    QDateTime timeEnd;
    QString roomNum;
    QString userID;
    QString windVelocity;
    float fee;
    float consumption;
    float totalfee;
    float totalconsum;
    bool isFirstRequest;
//add by wang 2017/6/4
    //用于报表处理

    int onoffTimes;
    QString sockettime;
    QString beginTime;
    QString endTime;
    int startTemperature;
    int endTemperature;
    QString startWind;
    QString endWind;

    int socketDay;
    int socketWeek;
    int socketMonth;
    int socketYear;
    int socketHour;
    int socketMinute;
    int socketSecond;

    QString roomTemp;
    QString targetTemp;
};

#endif // TCPSOCKET_H
