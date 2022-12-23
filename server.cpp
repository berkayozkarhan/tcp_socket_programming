#include "server.h"

Server::Server(QObject *parent) : QTcpServer{parent}
{
    pool.setMaxThreadCount(20);
}

void Server::start(quint16 port)
{
    qInfo() << this << " start " << QThread::currentThread();
    if(this->listen(QHostAddress::Any, port)) {
        qInfo() << "Server started on " << port;
    }else {
        qCritical() << this->errorString();
    }
}

void Server::quit()
{
    this->close();
    qInfo() << "Server Stopped.";
}

void Server::incomingConnection(qintptr handle)
{
    qInfo() << "Incoming connection " << handle << " on " << QThread::currentThread();
    Client *client = new Client(handle, nullptr); //it's going to run on another thread so parent is null ptr.
    client->setAutoDelete(true);
    pool.start(client);

}
