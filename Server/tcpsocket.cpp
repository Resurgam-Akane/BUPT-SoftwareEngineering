#include "tcpsocket.h"
#include "tcpserver.h"
#include <QtConcurrent/QtConcurrent>
#include <QHostAddress>
#include <QDebug>
#include <QTimer>
#include <QRegExp>
#include "globalvar.h"
#include"coldorwarm.h"
TcpSocket::TcpSocket(qintptr socketDescriptor, QObject *parent) : //构造函数在主线程执行，lambda在子线程
    QTcpSocket(parent),socketID(socketDescriptor)
{
    isFirstRequest = true;
    windVelocity = "0";
    fee = 0;
    consumption = 0;
    totalconsum = 0;
    totalfee = 0;
    this->setSocketDescriptor(socketDescriptor);
    connect(this,&TcpSocket::readyRead,this,&TcpSocket::readData);

    //todo:此处的频率可以设定，使用全局变量，放在一个名字空间里
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&](){
       this->write("request");
    });

    //add by xuzhu 2017/5/27
    timerCost = new QTimer(this);
    connect(timerCost, &QTimer::timeout, this, [&](){
        QDateTime curTime = QDateTime::currentDateTime();
        int workSec = timeStart.secsTo(curTime);

        //计算
        if (windVelocity == "0") {
            consumption = workSec * 0.8 * FACTOR;
            fee = consumption * 5;
            totalfee += fee;
            totalconsum += consumption;
        }
        else if (windVelocity == "1") {
            consumption = workSec * 1 * FACTOR;
            fee = consumption * 5;
            totalfee += fee;
            totalconsum += consumption;
        }
        else {
            consumption = workSec * 1.3 * FACTOR;
            fee = consumption * 5;
            totalfee += fee;
            totalconsum += consumption;
        }
        QString data = "cost/" + QString("%1").arg(totalconsum) +"/"+ QString("%1").arg(totalfee);
        this->write(data.toUtf8());
        qDebug() << "send data:" << data;
    });


    dis = connect(this,&TcpSocket::disconnected,
        [&](){
            qDebug() << "disconnect";
            timer->stop();
            timerCost->stop();

            TcpServer::loadTcpClientMtx->lock();
            TcpServer::loadTcpClient->remove(roomNum);
            TcpServer::loadTcpClientMtx->unlock();

            TcpServer::runListMtx->lock();
            TcpServer::waitListMtx->lock();
            TcpServer::LoadAvgMtx->lock();

            TcpServer::runList->remove_if([&](std::tuple<QString, int, int> n){return std::get<0>(n) == roomNum;});
            TcpServer::waitList->remove_if([&](std::tuple<QString, int, int> n){return std::get<0>(n) == roomNum;});
            TcpServer::LoadAvg(TcpServer::myself);
            TcpServer::LoadAvgMtx->unlock();
            TcpServer::waitListMtx->unlock();
            TcpServer::runListMtx->unlock();

            QSqlDatabase db = QSqlDatabase::database("QSQLITE");
            QSqlQuery query(db);
            query.prepare("DELETE FROM airstate WHERE roomnum = ?");
            query.addBindValue(roomNum);
            query.exec();
            emit this->reFresh();
            qDebug() << "refresh";

            emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort(),QThread::currentThread());//发送断开连接的用户信息
            this->deleteLater();
        });
    connect(&watcher,&QFutureWatcher<QByteArray>::finished,this,&TcpSocket::startNext);
    connect(&watcher,&QFutureWatcher<QByteArray>::canceled,this,&TcpSocket::startNext);
    qDebug() << "new connect" ;

    //add by wang
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
    //qDebug() << "发送数据:" << data;
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
            handleData = data.mid(0, 19);
            data = data.mid(19);
            roomNum = handleData.mid(10, 4);
            userID = handleData.mid(15);
            TcpServer::loadTcpClientMtx->lock();
            if (TcpServer::loadTcpClient->contains(roomNum))
                reData = "serverMsg/reject";
            else if(Cold_Or_Warm==0) //制冷模式
            {
                TcpServer::loadTcpClient->insert(roomNum, this);
                timer->start(900);
                reData = "serverMsg/0/18/25";
                sqlsetcoldstate();
                emit this->reFresh();
                qDebug() << "refresh";
            }else if(Cold_Or_Warm==1)//制热模式
            {
                TcpServer::loadTcpClient->insert(roomNum, this);
                timer->start(900);
                reData = "serverMsg/1/25/30";
                sqlsethotstate();
                emit this->reFresh();
                qDebug() << "refresh";
            }
            TcpServer::loadTcpClientMtx->unlock();
            sqlonoffTimes();           
        }
        else if (data.left(10) == "requestEnd")                     //todo::从runlist拿出，再运行负载均衡模块
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
                consumption += workSec * 0.8 * FACTOR;
                fee = consumption * 5;
                totalfee += fee;
                totalconsum += consumption;
            }
            else if (windVelocity == "1") {
                consumption += workSec * 1 * FACTOR;
                fee = consumption * 5;
                totalfee += fee;
                totalconsum += consumption;
            }
            else {
                consumption = workSec * 1.3 * FACTOR;
                fee = consumption * 5;
                totalfee += fee;
                totalconsum += consumption;
            }
            //QString data = "cost/" + QString("%1").arg(consumption) + QString("%2").arg(fee);
            QString con = QString("%1").arg(totalconsum);
            QString fe = QString("%2").arg(totalfee);

            TcpServer::runListMtx->lock();
            TcpServer::waitListMtx->lock();
            TcpServer::LoadAvgMtx->lock();
            //qDebug() << "exit" << "                         " << roomNum;
            TcpServer::runList->remove_if([&](std::tuple<QString, int, int> n){return std::get<0>(n) == roomNum;});
            TcpServer::LoadAvg(TcpServer::myself);

            TcpServer::LoadAvgMtx->unlock();
            TcpServer::waitListMtx->unlock();
            TcpServer::runListMtx->unlock();

            reData = "cost/" + con.toUtf8() + "/" + fe.toUtf8();
            qDebug() << "send data:" << reData;
            sqlstartend();
        }
        else if (data.left(4) == "wind")            //haha:将更新的风速写入数据库
        {
            handleData = data.mid(0,6);
            data = data.mid(6);
            //qDebug() << handleData;
            windVelocity = handleData[handleData.length()-1];
            //qDebug() << windVelocity;
            //负载允许的条件下，发送acceptWind
            sqlsetwind();
            emit reFresh();
            qDebug() << "refresh";
            bool isInrunList = false;
            bool isInwaitList = false;
            TcpServer::runListMtx->lock();
            TcpServer::waitListMtx->lock();
            TcpServer::LoadAvgMtx->lock();
            for (auto it = TcpServer::runList->begin(); it != TcpServer::runList->end() && !isInrunList; ++it) {
                if (std::get<0>(*it) == roomNum)
                    isInrunList = true;
            }
            auto it = TcpServer::waitList->begin();
            for (; it != TcpServer::waitList->end() && !isInwaitList; ++it) {
                if (std::get<0>(*it) == roomNum)
                    isInwaitList = true;
            }
            TcpServer::LoadAvgMtx->unlock();
            TcpServer::waitListMtx->unlock();
            TcpServer::runListMtx->unlock();

            if (isInrunList) ;//在runlist里，什么都不干
            else if (!isInrunList && isInwaitList) {//修改waitList对应元素的内容，并且调用负载均衡模块
                int order = std::get<2>(*it);
                TcpServer::runListMtx->lock();
                TcpServer::waitListMtx->lock();
                TcpServer::LoadAvgMtx->lock();
                TcpServer::waitList->push_back(std::make_tuple(roomNum, windVelocity.toInt(), order));

                TcpServer::LoadAvg(TcpServer::myself);

                TcpServer::LoadAvgMtx->unlock();
                TcpServer::waitListMtx->unlock();
                TcpServer::runListMtx->unlock();
            }
            else if (!isInrunList && !isInwaitList) ; //不在runlist也不再waitlist，什么都不干

            reData = "acceptWind";
            //qDebug()<<"-------fengsu---------";
            sqlendWindchange();
        }
        else if (data.left(7) == "request")             //haha:将用户新请求，包括当前温度和目标温度，写入数据库，用户当前的状态为在等待队列 wait
        {
            QString words(data);
            QString requestPattern("request\/([0-9]+(\.[0-9]+)?)\/([0-9]+(\.[0-9]+)?)");
            QRegExp rx(requestPattern);

            int pos = words.indexOf(rx);
            if (pos < 0) return "";
            data = rx.cap(5).toLatin1();
            roomTemp = rx.cap(1);     //当前室温
            targetTemp = rx.cap(3);  //目标温度
            sqlsetwaitstate();
            emit this->reFresh();
            sqlstartWindchange();
            if (isFirstRequest) {
                isFirstRequest = false;
                timerCost->start(60000);
                timeStart = QDateTime::currentDateTime();
                QString cur = QDateTime::currentDateTime().toString(Qt::ISODate); //第一次request的时间

                TcpServer::runListMtx->lock();
                TcpServer::waitListMtx->lock();
                TcpServer::LoadAvgMtx->lock();
                //qDebug() << "enter waitlist" << "                         " << roomNum;
                TcpServer::waitList->push_back(std::make_tuple(roomNum, 0, int(TcpServer::waitList->size())));
                //todo::waitList排序


                TcpServer::LoadAvg(TcpServer::myself);

                TcpServer::LoadAvgMtx->unlock();
                TcpServer::waitListMtx->unlock();
                TcpServer::runListMtx->unlock();
                //将第一次request的时间写入数据库 记得加上新的目标温度
            }
            else {
                QString cur = QDateTime::currentDateTime().toString(Qt::ISODate); //第二、三等次request的时间，此类情况是室内温度没有达到目标温度时，又改变了目标温度
                //将第二、三次request的时间写入数据库 记得加上新的目标温度
                bool  isInrunList = false;
                TcpServer::runListMtx->lock();
                TcpServer::waitListMtx->lock();
                TcpServer::LoadAvgMtx->lock();
                for (auto it = TcpServer::runList->begin(); it != TcpServer::runList->end() && !isInrunList; ++it) {
                    if (std::get<0>(*it) == roomNum)
                        isInrunList = true;
                }

                if (!isInrunList) {                 //如果不在runlist里，则运行负载均衡模块；如果在runlist里，则什么都不做，继续让从控机运行
                    TcpServer::LoadAvg(TcpServer::myself);
                }
                else {
                    this->write("answer");
                    sqlsetrunstate();
                }
                TcpServer::LoadAvgMtx->unlock();
                TcpServer::waitListMtx->unlock();
                TcpServer::runListMtx->unlock();

                sqlendTempchange(targetTemp);
            }

            //todo:在负载允许的条件下，返回answer。 记录下回anwser的时间
             //reData = "answer";

        }
        else if (data.left(6) == "answer")      //haha:使用当前温度rx.cap(1)和目标温度rx.cap(2)
        {                                       //haha:将房间号，用户ID，当前温度，目标温度写到数据库里
            QString words(data);
            QString answerPattern("answer\/([0-9]+(\.[0-9]+)?)\/([0-9]+(\.[0-9]+)?)\/(0|1|2)");
            QRegExp rx(answerPattern);

            int pos = words.indexOf(rx);
            if (pos < 0) return "";
            data = rx.cap(6).toLatin1();

 //           qDebug() << rx.capturedTexts(); //("answer/25/22/0", "25", "22", "0")
            reData = "";
            qDebug() << rx.cap(1) << " ------------" << rx.cap(3);
            sqlsetanwstate(rx.cap(1),rx.cap(3));
            emit this->reFresh();
            //qDebug() << "refresh";
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

void TcpSocket::sqlonoffTimes(){
//    comTime::calculateTime(&socketYear,&socketMonth,&socketDay,&socketHour,&socketMinute,&socketSecond,&socketWeek);
//    sockettime = QString::number(socketYear) + "-" + QString::number(socketMonth) + "-" + QString::number(socketDay) +
//                        "-" + QString::number(socketHour) + ":" + QString::number(socketMinute) + ":" + QString::number(socketSecond);
    sockettime = mimicTime();
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("SELECT roomnum FROM roomfee WHERE roomnum = ? ");
    query.addBindValue(roomNum);
    query.exec();
    int num = 0;
    while(query.next()){
        num++;
        //qDebug()<<query.value(0).toString();
    }
    if(!num){
        query.clear();
        query.prepare("insert into roomfee (roomnum, onofftimes, starttime, endtime, starttemperature, endtemperature, startwind, endwind, fee, newesttime)"
                      "VALUES(:roomnum, :onofftimes, :starttime, :endtime, :starttemperature, :endtemperature, :startwind,:endwind, :fee, :newesttime)");
        query.bindValue(":roomnum", roomNum);
        query.bindValue(":onofftimes", 1);
        query.bindValue(":starttime", sockettime);
        query.bindValue(":endtime", sockettime);
        query.bindValue(":starttemperature", "20");
        query.bindValue(":endtemperature", "20");
        query.bindValue(":startwind", "0");
        query.bindValue(":endwind","0");
        query.bindValue(":fee",0);
        query.bindValue(":newesttime", 1);
        query.exec();
    }
    else{
        int thetimes = 0;
        query.clear();
        query.prepare("SELECT onofftimes FROM roomfee WHERE roomnum = ? ");
        query.addBindValue(roomNum);
        query.exec();
        while(query.next()){
            thetimes = query.value(0).toInt();
        }

        thetimes++;

        query.clear();
        query.prepare("UPDATE roomfee SET onofftimes = ?, newesttime = ? WHERE roomnum = ?");
        query.bindValue(0, thetimes);
        query.bindValue(1, 0);
        query.bindValue(2, roomNum);
        query.exec();

        query.clear();
        query.prepare("insert into roomfee (roomnum, onofftimes, starttime, endtime, starttemperature, endtemperature, startwind, endwind, fee, newesttime)"
                      "VALUES(:roomnum, :onofftimes, :starttime, :endtime, :starttemperature, :endtemperature, :startwind,:endwind, :fee, :newesttime)");
        query.bindValue(":roomnum", roomNum);
        query.bindValue(":onofftimes", thetimes);
        query.bindValue(":starttime", sockettime);
        query.bindValue(":endtime", sockettime);
        query.bindValue(":starttemperature", "20");
        query.bindValue(":endtemperature", "20");
        query.bindValue(":startwind", 0);
        query.bindValue(":endwind",0);
        query.bindValue(":fee",0);
        query.bindValue(":newesttime",1);
        query.exec();
    }
}

void TcpSocket::sqlstartend(){
//    comTime::calculateTime(&socketYear,&socketMonth,&socketDay,&socketHour,&socketMinute,&socketSecond,&socketWeek);
//    sockettime = QString::number(socketYear) + "-" + QString::number(socketMonth) + "-" + QString::number(socketDay) +
//                        "-" + QString::number(socketHour) + ":" + QString::number(socketMinute) + ":" + QString::number(socketSecond);
    sockettime = mimicTime();
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("UPDATE roomfee SET endtime = ?, fee = ?, newesttime = ? WHERE roomnum = ? and newesttime = ?");
    query.bindValue(0, sockettime);
    query.bindValue(1, fee);
    query.bindValue(2, 0);
    query.bindValue(3, roomNum);
    query.bindValue(4, 1);
    query.exec();
    QSqlQuery query2(db);
    int lateday = 0;
    int weekday = 0;
    int latemonth;
    query2.clear();
    query2.exec("select month,day,week from roomadjust");
    while(query2.next()){
        latemonth = query2.value(0).toInt();
        lateday = query2.value(1).toInt();
        weekday = query2.value(2).toInt();
    }
    if(weekday == 0){
        query2.prepare("insert into roomadjust (roomnum, date, expense, month, day, hour, min,week)"
                      "VALUES(:roomnum, :date, :expense, :month, :day, :hour, :min,:week)");
        query2.bindValue(0, roomNum);
        query2.bindValue(1, sockettime);
        query2.bindValue(2, fee);
        query2.bindValue(3, sockettime.section('-',0,0).toInt());
        query2.bindValue(4, sockettime.section('-',1,1).toInt());
        query2.bindValue(5, sockettime.section('-',2,2).toInt());
        query2.bindValue(6, sockettime.section('-',3,3).toInt());
        query2.bindValue(7, 1);
        query2.exec();
    }
    else{
        int judgeweek = weekday + ((sockettime.section('-',0,0).toInt() - latemonth) * 30 + (sockettime.section('-',1,1).toInt() - lateday))/7;
        query2.prepare("insert into roomadjust (roomnum, date, expense, month, day, hour, min,week)"
                      "VALUES(:roomnum, :date, :expense, :month, :day, :hour, :min,:week)");
        query2.bindValue(0, roomNum);
        query2.bindValue(1, sockettime);
        query2.bindValue(2, fee);
        query2.bindValue(3, sockettime.section('-',0,0).toInt());
        query2.bindValue(4, sockettime.section('-',1,1).toInt());
        query2.bindValue(5, sockettime.section('-',2,2).toInt());
        query2.bindValue(6, sockettime.section('-',3,3).toInt());
        query2.bindValue(7, judgeweek);
        query2.exec();
    }
    query.clear();
    query.prepare("UPDATE airstate SET roomtemp = ? ,targettemp = ?,runstate = ? WHERE roomnum = ? and username = ?");
    query.addBindValue(targetTemp);
    query.addBindValue(targetTemp);
    query.addBindValue("stand by");
    query.addBindValue(roomNum);
    query.addBindValue(userID);
    query.exec();
}

void TcpSocket::sqlstartWindchange(){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("UPDATE roomfee SET startwind = ? WHERE roomnum = ? and newesttime = ?");
    query.addBindValue(windVelocity);
    query.addBindValue(roomNum);
    query.addBindValue(1);
    if(!query.exec())   qDebug()<<"511"<<query.lastError();
}

void TcpSocket::sqlstartTempchange(QString Ctemp){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.clear();
    query.prepare("UPDATE roomfee SET starttemperature = ? WHERE roomnum = ? and newesttime = ?");
    query.bindValue(0, Ctemp);
    query.bindValue(1, roomNum);
    query.bindValue(2, 1);
    query.exec();
}

void TcpSocket::sqlendWindchange(){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("UPDATE roomfee SET endwind = ? WHERE roomnum = ? and newesttime = ?");
    query.bindValue(0, windVelocity);
    query.bindValue(1, roomNum);
    query.bindValue(2, 1);
    if(!query.exec())   qDebug()<<"532"<<query.lastError();
}

void TcpSocket::sqlendTempchange(QString Ctemp){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("UPDATE roomfee SET endtemperature = ? WHERE roomnum = ? and newesttime = ?");
    query.bindValue(0, Ctemp);
    query.bindValue(1, roomNum);
    query.bindValue(2, 1);
    query.exec();
}

void TcpSocket::sqlnewRequire(QString Ctemp1,QString Ctemp2){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    sockettime = mimicTime();
    query.prepare("SELECT onofftimes, newesttime FROM roomfee WHERE roomnum = ? ");
    query.addBindValue(roomNum);
    query.exec();
    int num = 0;
    int ifnew = 0;
    while(query.next()){
       num = query.value(0).toInt();
       ifnew = query.value(1).toInt();
    }
    query.clear();
    if(ifnew == 0){
        query.clear();
        query.prepare("insert into roomfee (roomnum, onofftimes, starttime, endtime, starttemperature, endtemperature, startwind, endwind, fee, newesttime)"
                      "VALUES(:roomnum, :onofftimes, :starttime, :endtime, :starttemperature, :endtemperature, :startwind,:endwind, :fee, :newesttime)");
        query.bindValue(":roomnum", roomNum);
        query.bindValue(":onofftimes", num);
        query.bindValue(":starttime", sockettime);
        query.bindValue(":endtime", sockettime);
        query.bindValue(":starttemperature", Ctemp1);
        query.bindValue(":endtemperature", Ctemp2);
        query.bindValue(":startwind", windVelocity);
        query.bindValue(":endwind",windVelocity);
        query.bindValue(":fee",0);
        query.bindValue(":newesttime",1);
        if(!query.exec())   qDebug()<<"573"<<query.lastError();
    }
    else{
        query.clear();
        query.exec("UPDATE roomfee SET starttemperature = ?, endtemperature = ? WHERE roomnum = ? and newesttime = ?");
        query.addBindValue(Ctemp1);
        query.addBindValue(Ctemp2);
        query.addBindValue(roomNum);
        query.addBindValue(1);
        if(!query.exec())   qDebug()<<"582"<<query.lastError();
    }
}

QString TcpSocket::mimicTime(){
    QString nowtime = QDateTime::currentDateTime().toString(Qt::ISODate);//2017-06-06T01:49:37
    nowtime = nowtime.section('T',1,1);
    int nowhour = nowtime.section(':',0,0).toInt();
    int nowminute = nowtime.section(':',1,1).toInt();
    int nowsecond = nowtime.section(':',2,2).toInt();
    int mmonth,mday,mhour,mmin;
    mmonth = nowhour/2 + 1;
    mday = (nowhour % 2) * 15 + (nowminute/2) + 1;
    mhour = (nowminute % 2) * 12 + (nowsecond/60) * 24;
    mmin = (nowsecond * 24) % 60;
    if(mhour >= 24){
        mhour = mhour -24;
        mday++;
    }
    if(mday > 30){
        mday = mday -30;
        mmonth++;
    }
    return QString::number(mmonth) + "-" + QString::number(mday) + "-" +QString::number(mhour) + "-" + QString::number(mmin);
}

void TcpSocket::sentAnswer(TcpSocket * i)
{
    if (i == this){
        this->write("answer");
        sqlnewRequire(roomTemp,targetTemp);     //haha:更新当前用户的状态为running
        sqlsetrunstate();
        emit this->reFresh();
        qDebug() << "refresh";
    }
}

void TcpSocket::sqlsetanwstate(QString Ctemp1, QString Ctemp2){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("UPDATE airstate SET roomtemp = ? ,targettemp = ? WHERE roomnum = ? and username = ?");
    query.addBindValue(Ctemp1);
    query.addBindValue(Ctemp2);
    query.addBindValue(roomNum);
    query.addBindValue(userID);
    if(!query.exec())   qDebug()<<"627"<<query.lastError();
}

void TcpSocket::sqlsethotstate(){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("insert into airstate (roomnum, username, roomtemp, targettemp, wind, runstate)"
                  "VALUES(:roomnum, :username, :roomtemp, :targettemp, :wind, :runstate)");
    query.bindValue(":roomnum", roomNum);
    query.bindValue(":username", userID);
    query.bindValue(":roomtemp", "25");
    query.bindValue(":targettemp", "28");
    query.bindValue(":wind","0");
    query.bindValue(":runstate", "stand by");
    if(!query.exec())   qDebug()<<"641"<<query.lastError();
}

void TcpSocket::sqlsetcoldstate(){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("insert into airstate (roomnum, username, roomtemp, targettemp, wind, runstate)"
                  "VALUES(:roomnum, :username, :roomtemp, :targettemp, :wind, :runstate)");
    query.bindValue(":roomnum", roomNum);
    query.bindValue(":username", userID);
    query.bindValue(":roomtemp", "25");
    query.bindValue(":targettemp", "22");
    query.bindValue(":wind","0");
    query.bindValue(":runstate", "standby");
    if(!query.exec())   qDebug()<<"655"<<query.lastError();
}

void TcpSocket::sqlsetrunstate(){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("UPDATE airstate SET runstate = ? WHERE roomnum = ? and username = ?");
    query.addBindValue("running");
    query.addBindValue(roomNum);
    query.addBindValue(userID);
    query.exec();
}

void TcpSocket::sqlsetwaitstate(){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("UPDATE airstate SET roomtemp = ? ,targettemp = ?,runstate = ? WHERE roomnum = ? and username = ?");
    query.addBindValue(roomTemp);
    query.addBindValue(targetTemp);
    query.addBindValue("stand by");
    query.addBindValue(roomNum);
    query.addBindValue(userID);
    query.exec();
}

void TcpSocket::sqlsetwind(){
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("UPDATE airstate SET wind = ? WHERE roomnum = ? and username = ?");
    query.addBindValue(windVelocity);
    query.addBindValue(roomNum);
    query.addBindValue(userID);
    query.exec();
}
