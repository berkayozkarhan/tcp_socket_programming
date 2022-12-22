#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include "clienthandler.h"

#define LOG qDebug()

/*

namespace Commands {
    const static int Register = 1;
    const static int Login = 2;
    const static int TransferMoney = 3;
}

*/


#define CMD_REGISTER 1
#define CMD_LOGIN 2
#define CMD_TRANSFER_MONEY 3

class Server : public QObject
{
    Q_OBJECT

private:
    QString server_type = "";
    int port = 0;
    QTcpServer *server;
    void init_server();

public:
    explicit Server(QObject *parent = nullptr);

    //getter
    QString get_server_type() { return this->server_type; }
    int     getPort() const { return port; }

    //setter
    void set_server_config(QString config)
    {    if(config.isEmpty() || config.isNull())
        {
            LOG << "Invalid server type";
            return;
        }

        this->server_type = config;
    }
    void setPort(int port) { this->port = port; }
    void start_server(); // Threaded

public slots:
    void newConnection();
signals:

};

#endif // SERVER_H
