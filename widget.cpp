#include "widget.h"
#include "ui_widget.h"
#include "QTcpServer"
#include "QTcpSocket"
#include "QVBoxLayout"
#include "QProgressBar"
#include "QThread"
#include "QHBoxLayout"
#include "QListWidgetItem"
#include "QListWidget"
#include "tcpserver.h"
#include "QHostAddress"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *headerLayout = new QHBoxLayout;
    startButton = new QPushButton(this);
    startButton->setText("Start");
    connect(startButton, &QPushButton::clicked, this, &Widget::start);

    stopButton = new QPushButton(this);
    stopButton->setText("Stop");
    stopButton->setEnabled(false);
    connect(stopButton, &QPushButton::clicked, this, &Widget::stop);

    headerLayout->addWidget(startButton);
    headerLayout->addWidget(stopButton);

    listView = new QListWidget(this);
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(listView);
    this->setLayout(mainLayout);
}

Widget::~Widget()
{
}

void Widget::start()
{
    qDebug()<<"start to listen";
    tcpserver = new TcpServer(this);
    if(tcpserver->listen(QHostAddress::LocalHost, 56789))
    {
        connect(tcpserver, SIGNAL(bytesArrived(qint64,qint64,int)), this, SLOT(updateProgressStatus(qint64,qint64,int)));
        stopButton->setEnabled(true);
        startButton->setEnabled(false);
    }
    else
    {
        qDebug()<<tcpserver->errorString();
        return;
    }
}

void Widget::stop()
{
    qDebug()<<"Stop";
    tcpserver->close();
    stopButton->setEnabled(false);
    startButton->setEnabled(true);
}

void Widget::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"displayError:"<<tcpserver->errorString();
    tcpserver->close();
}


//void Widget::feedBack()
//{
//    qDebug()<<"return info to Client";
//    QString validateResult = validate();

//    QDataStream out(&outBlock, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_6);
//    out<<validateResult;
//    tcpConnection->write(outBlock);
//    outBlock.resize(0);
//    tcpConnection->close();
//}

void Widget::updateProgressStatus(qint64 bytesReceived, qint64 totalSize, int socketId)
{
    qDebug()<<"updateProgress Status";
    qDebug()<<"socketId = "<<socketId;
    qDebug()<<"progressbarMap size = "<<progressbarMap.size();
    qDebug()<<"bytesReceived = "<<bytesReceived;
    qDebug()<<"totalSize = "<<totalSize;
    mutex.lock();
    if(progressbarMap.size() == 0 || !progressbarMap.contains(socketId))
    {
        qDebug()<<"     create new ProgressBar";
        QProgressBar *progressBar = new QProgressBar(this);
        progressBar->setMaximum(totalSize);
        progressBar->setValue(bytesReceived);
        progressbarMap.insert(socketId, progressBar);
        QListWidgetItem *item = new QListWidgetItem(listView);
        item->setSizeHint(progressBar->size());
        listView->setItemWidget(item, progressBar);
    }
    else
    {
        qDebug()<<"     update exists progress bar";
        QProgressBar *progressBar = progressbarMap.value(socketId);
        progressBar->setMaximum(totalSize);
        progressBar->setValue(bytesReceived);
        progressbarMap.remove(socketId);
        progressbarMap.insert(socketId, progressBar);

//        if(totalSize == bytesReceived)
//        {
//            progressbarMap.remove(socketId);
//            refreshProgressBarListView();
//        }
    }
    mutex.unlock();
}

void Widget::refreshProgressBarListView()
{
    qDebug()<<"refresh Progress Bar List View";
    qDebug()<<"progressbarMap Size = "<<progressbarMap.size();
    listView->clear();
    if(progressbarMap.size()>0)
    {
        for(QMap<int, QProgressBar*>::iterator it = progressbarMap.begin();
            it != progressbarMap.end(); ++it)
        {
            QListWidgetItem *item = new QListWidgetItem(listView);
            item->setSizeHint(it.value()->size());
            listView->setItemWidget(item, it.value());
        }
    }
}






