#include "tcpserver.h"
#include <QDebug>
#include <QTcpSocket>


TcpServer::TcpServer(QObject *parent, QString ip, quint16 port):QTcpServer(parent)
{
    QHostAddress address(ip);
    this->listen(address,port);
}

TcpServer::~TcpServer()
{
    emit deleteClient();
    clientlist.clear();
}

quint16 TcpServer::qlistCount()
{
    return  clientlist.count();
}

void TcpServer::sendMessage(QByteArray msg, int i)
{
    temp=new QTcpSocket(this);
    temp=clientlist.at(i);
    temp->write(msg);
}

void TcpServer::incomingConnection(qintptr handle)
{
    client=new QTcpSocket(this);
    client->setSocketDescriptor(handle);

    clientlist.append(client);
    emit newClientInfo();

    connect(client,&QTcpSocket::readyRead,this,&TcpServer::slotUpdateServer);
    connect(client,&QTcpSocket::disconnected,this,&TcpServer::slotClientDisconnected);
}
void TcpServer::slotUpdateServer()
{
    QByteArray array = client->readAll();
    QString msg=array;
    qDebug()<<"recv msg:"<<msg;
    emit updataServer(msg);
}

void TcpServer::slotClientDisconnected()
{
    qDebug()<<"TcpServer::slotClientDisconnected()"<<clientlist.count();
   emit deleteClient();
   clientlist.clear();
}
