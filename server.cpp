#include "server.h"
#include <QTcpSocket>
Server::Server(QObject *parent)
    : QObject{parent}
{

}

void Server::init_server()
{
    if(server_type == "tcp")
        server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()),
                this, SLOT(newConnection()));


}

void Server::start_server()
{
    init_server();

    if(!server->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void Server::newConnection()
{
    LOG << "New connection ! ";
    QTcpSocket *socket = server->nextPendingConnection();
    QString newAddress;
    if(socket->waitForConnected()) {
        LOG << "Connecting...";
        newAddress = socket->peerAddress().toString().split(':')[3];
    }
}


