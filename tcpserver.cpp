#include "tcpserver.h"
#include "filesavethread.h"
#include "QThread"
#include "QTcpSocket"

TcpServer::TcpServer(QObject *parent)
    :QTcpServer(parent)
{
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"new Connection coming";

    QTcpSocket *tcpSocket = new QTcpSocket;
    tcpSocket->setSocketDescriptor(socketDescriptor);
    emit this->newConnection();
    if(tcpSocket->peerAddress() == QHostAddress::LocalHost)
    {
        fileSaveThread *thread = new fileSaveThread(socketDescriptor, this);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        connect(thread, &fileSaveThread::bytesArrived, this, &TcpServer::bytesArrived);
        thread->start();
    }
    else
        return;
}
