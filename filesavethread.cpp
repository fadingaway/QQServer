#include "filesavethread.h"
#include "QDataStream"
#include "QThread"
#include "QApplication"
#include "QTcpSocket"

fileSaveThread::fileSaveThread(qintptr socketDescriptor,
                               QObject *parent)
    :QThread(parent), socketDescriptor(socketDescriptor)
{
    qDebug()<<"thread created";
    bytesReceived = 0;
}

void fileSaveThread::run()
{
    qDebug()<<"thread run";
    tcpSocket = new QTcpSocket;
    //tcpSocket->setReadBufferSize(1024);
    if(!tcpSocket->setSocketDescriptor(socketDescriptor))
    {
        qDebug()<<tcpSocket->errorString();
        return;
    }
    qDebug()<<"current socket State = "<<tcpSocket->state();
    if(tcpSocket->waitForConnected())
    {
        connect(tcpSocket, &QTcpSocket::readyRead, this, &fileSaveThread::receiveFile, Qt::BlockingQueuedConnection);
        connect(tcpSocket, &QTcpSocket::disconnected, this, &QThread::quit);
    }
    else
    {
        qDebug()<<"connect failed.";
    }
    this->exec();
}

void fileSaveThread::receiveFile()
{
    qDebug()<<"----------start------------";
    qDebug()<<"receive File start";
    qDebug()<<"current socket State = "<<tcpSocket->state();
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_7);

    if(bytesReceived < sizeof(qint64)*2)
    {
        qDebug()<<"inComing bytes = "<<tcpSocket->bytesAvailable();
        qDebug()<<"fileNameSize = "<<fileNameSize;
        if((tcpSocket->bytesAvailable()>= sizeof(qint64)*2)
                &&fileNameSize == 0)
        {
            in>>totalBytes>>fileNameSize;
            bytesReceived += sizeof(qint64)*2;
            qDebug()<<"totalBytes = "<<totalBytes;
            qDebug()<<"fileNameSize = "<<fileNameSize;
        }

        if((tcpSocket->bytesAvailable() >= fileNameSize)
                &&fileNameSize != 0)
        {
            in>>fileName;
            qDebug()<<"fileName = "<<fileName;
            bytesReceived += fileNameSize;

            if(!fileName.isEmpty())
            {
                localFile = new QFile(fileName);
                if(!localFile->open(QFile::WriteOnly))
                {
                    qDebug()<<localFile->errorString();
                    localFile->close();
                    this->quit();
                    this->wait();
                    return;
                }
            }
            else
            {
                this->quit();
                this->deleteLater();
                return;
            }
        }
        else
            return;
    }

    qDebug()<<"bytesReceived = "<<bytesReceived;
    if(bytesReceived <totalBytes && tcpSocket->bytesAvailable())
    {
        qDebug()<<"bytesAvailable = "<<tcpSocket->bytesAvailable();
        bytesReceived += tcpSocket->bytesAvailable();
        qDebug()<<"a";
        inBlock = tcpSocket->read(1024);
        qDebug()<<"b";
        localFile->write(inBlock);
        qDebug()<<"c";
        inBlock.resize(0);
        qDebug()<<"write file done.";
    }

    emit bytesArrived(bytesReceived, totalBytes, socketDescriptor);
    if(bytesReceived == totalBytes)
    {
        localFile->close();
        this->quit();
        qDebug()<<"file receive complete.";
    }

    qDebug()<<"current socket State = "<<tcpSocket->state();
    qDebug()<<"receive File end";
    qDebug()<<"----------end------------";
}
