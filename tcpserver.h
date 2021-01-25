#ifndef TCPSERVER_H
#define TCPSERVER_H
#pragma once

#include <QTcpServer>
#include <QThread>

class TcpServer:public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent=0,QString ip="",quint16 port=0);
    ~TcpServer();
    quint16 qlistCount();
    void sendMessage(QByteArray msg,int i);

protected:
    virtual void incomingConnection(qintptr handle) override;
    QList<QTcpSocket*> clientlist;
    QTcpSocket *client;
    QTcpSocket *temp;
    QTcpSocket *deleteSocket=nullptr;
private slots:
    void slotUpdateServer();
    void slotClientDisconnected();
signals:
    void updataServer(QString);
    void newClientInfo();
    void deleteClient();
    void clientDisconnected(qintptr);

private:
    QStringList m_oData;
    QString listenip;
    quint16 listenport;
};
//class Msgthreadserver :public QObject
//{
//    Q_OBJECT
//    Msgthreadserver(QString host="",QObject *parent=nullptr);
//    ~Msgthreadserver();
//private:
//    QThread thread1;
//    QThread thread2;
//    TcpServer* server1;
//    TcpServer* server2;
//};

#endif // TCPSERVER_H
