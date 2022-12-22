#include <QCoreApplication>
#include <QHttpServer>
#include <QtConcurrent>
#include <QDebug>
#include "server.h"
#include "globals.h"
/*
 * :: API ::
:/register :
    @param name : str
    @param surname : str
    @param identity_no: str (pass)
    @param username: str
    @param password: str
    @param token : str

:/transfer
    token: str
    account_number: str



*/


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QHttpServer httpServer;

    Server *s = new Server();
    s->set_server_config(G::Server::config);
    s->setPort(G::Server::port);
    qDebug() << "config : " << s->get_server_type();
    qDebug() << "port : " << s->getPort();
    s->start_server();



    return a.exec();
}
