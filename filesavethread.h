#ifndef FILESAVETHREAD_H
#define FILESAVETHREAD_H

#include <QObject>
#include "QTcpSocket"
#include "QFile"
#include "QTcpServer"
#include "tcpserver.h"
#include "QThread"

class QFile;
class QTcpSocket;
class tcpServer;

class fileSaveThread : public QThread
{
    Q_OBJECT
public:
    fileSaveThread(qintptr socketDescriptor, QObject *parent);
    void run();
signals:
    void error(QTcpSocket::SocketError socketError);
    void bytesArrived(qint64, qint32, int);
public slots:
    void receiveFile();
private:
    qintptr socketDescriptor = 0;
    qint64 bytesReceived = 0;
    qint64 bytesToRead = 0;
    qint64 totalBytes = 0;
    qint64 fileNameSize = 0;
    QTcpSocket *tcpSocket;
    QString fileName;
    QFile *localFile;
    QByteArray inBlock;
    tcpServer *tcpserver;
};

#endif // FILESAVETHREAD_H
