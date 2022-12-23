#include <QCoreApplication>
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
    Server server;
    server.start(G::Server::port);
    return a.exec();
}
