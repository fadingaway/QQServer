#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "QTcpServer"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = 0);
signals:
    void bytesArrived(qint64, qint64, int);
protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
