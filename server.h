#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>
#include <QThread>
#include "client.h"
#include "database.h"
#include "globals.h"

#define LOG qDebug()

#define CMD_REGISTER 1
#define CMD_LOGIN 2
#define CMD_TRANSFER_MONEY 3

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
public slots:
    void start(quint16 port);
    void quit();
signals:


    // QTcpServer interface
protected:
    virtual void incomingConnection(qintptr handle) Q_DECL_OVERRIDE; // override.
private:
    QThreadPool pool;
    Database    *db;
};

#endif // SERVER_H
