#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QAbstractSocket"
#include "QTcpServer"
#include "QListWidget"
#include "QPushButton"
#include "tcpserver.h"
#include "QProgressBar"
#include "QMap"
#include "QMutex"
class TcpServer;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void start();
    void displayError(QAbstractSocket::SocketError socketError);
//    void feedBack();
    void stop();
    void updateProgressStatus(qint64, qint64, int);
    void refreshProgressBarListView();
private:
    QPushButton *startButton;
    QPushButton *stopButton;
    QListWidget *listView;
    TcpServer *tcpserver;
    QMap<int, QProgressBar *> progressbarMap;
    mutable QMutex mutex;
};

#endif // WIDGET_H
