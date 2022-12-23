#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>

const static char *Banner = "Welcome. Please specify operation described below. \n"
                            "register : New user registration\n"
                            "transfer_money : Transfer money.\n"
                            "operation : ";


class Client : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Client(qintptr handle = 0, QObject *parent = nullptr);

public slots:

signals:
    // QRunnable interface
public:
    void run();
    bool wait(int msecs);
private:
    qintptr handle;
    QTcpSocket *socket;

};

#endif // CLIENT_H
