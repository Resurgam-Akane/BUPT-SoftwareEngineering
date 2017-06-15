#include "tcpserver.h"
#include "threadhandle.h"
#include "globalvar.h"
#include <QMutex>

QMutex *TcpServer::loadTcpClientMtx = new QMutex;
QMap<QString, TcpSocket *> * TcpServer::loadTcpClient = new QMap<QString, TcpSocket *>;
std::list<std::tuple<QString, int, int>> *TcpServer::waitList = new std::list<std::tuple<QString, int, int>>;
std::list<std::tuple<QString, int, int>> *TcpServer::runList = new std::list<std::tuple<QString, int, int>>;
QMutex *TcpServer::waitListMtx = new QMutex;
QMutex *TcpServer::runListMtx = new QMutex;
QMutex *TcpServer::LoadAvgMtx = new QMutex;
TcpServer* TcpServer::myself = new TcpServer;

void TcpServer::LoadAvg(TcpServer * i)
{
    TcpServer::loadTcpClientMtx->lock();
    if (runList->size() < 3 && waitList->empty()) {
        ;
    }
    else if (runList->size() < 3 && !waitList->empty()) {
        while (runList->size() < 3 && !waitList->empty()) {
            std::tuple<QString, int, int> tmp = waitList->front();
            TcpServer::waitList->pop_front();                       //从等待队列取出
            qDebug() << "enter RUNLIST                 " << std::get<0>(tmp);
            TcpServer::runList->push_back(tmp);                    //加入运行队列

            QMap<QString, TcpSocket *>::iterator it = loadTcpClient->find(std::get<0>(tmp));
            emit i->sentAnwser(it.value());
            //it.value()->write("answer");            //返回answer
            //todo::数据库的地方要改
        }
    }
    TcpServer::loadTcpClientMtx->unlock();
}

TcpServer::TcpServer(QObject *parent,int numConnections) :
    QTcpServer(parent)
{
     tcpClient = new  QHash<int,TcpSocket *>;
     setMaxPendingConnections(numConnections);
     TcpServer::myself = this;
}

TcpServer::~TcpServer()
{
    QSqlDatabase db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query(db);
    query.prepare("drop TABLE airstate");
    emit this->sentDisConnect(-1);
    delete tcpClient;
}

void TcpServer::setMaxPendingConnections(int numConnections)
{
    this->QTcpServer::setMaxPendingConnections(numConnections);//调用Qtcpsocket函数，设置最大连接数，主要是使maxPendingConnections()依然有效
    this->maxConnections = numConnections;
}

void TcpServer::incomingConnection(qintptr socketDescriptor) //多线程必须在此函数里捕获新连接
{
    if (tcpClient->size() > maxPendingConnections())//继承重写此函数后，QTcpServer默认的判断最大连接数失效，自己实现
    {
        QTcpSocket tcp;
        tcp.setSocketDescriptor(socketDescriptor);
        tcp.disconnectFromHost();
        return;
    }
    auto th = ThreadHandle::getClass().getThread();
    auto tcpTemp = new TcpSocket(socketDescriptor);
    QString ip =  tcpTemp->peerAddress().toString();
    qint16 port = tcpTemp->peerPort();

    connect(tcpTemp,&TcpSocket::sockDisConnect,this,&TcpServer::sockDisConnectSlot);//NOTE:断开连接的处理，从列表移除，并释放断开的Tcpsocket，此槽必须实现，线程管理计数也是考的他
    connect(this,&TcpServer::sentDisConnect,tcpTemp,&TcpSocket::disConTcp);//断开信号
    connect(this, &TcpServer::sentAnwser, tcpTemp, &TcpSocket::sentAnswer);
    connect(tcpTemp, &TcpSocket::reFresh, this, [&](){
        emit this->reFresh();
    });

    tcpTemp->moveToThread(th);//把tcp类移动到新的线程，从线程管理类中获取
    tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中

    emit connectClient(socketDescriptor,ip,port);
}

void TcpServer::sockDisConnectSlot(int handle,const QString & ip, quint16 prot,QThread * th)
{
    tcpClient->remove(handle);//连接管理中移除断开连接的socket
    ThreadHandle::getClass().removeThread(th); //告诉线程管理类那个线程里的连接断开了
    emit sockDisConnect(handle,ip,prot);
}


void TcpServer::clear()
{
    emit this->sentDisConnect(-1);
    ThreadHandle::getClass().clear();
    tcpClient->clear();
}

