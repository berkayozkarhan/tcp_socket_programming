#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QRunnable>
#include <QThread>
#include <QTcpSocket>
#include "database.h"
#include <QRandomGenerator>


class Client : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Client(qintptr handle = 0, Database *db=nullptr, QObject *parent = nullptr);

public slots:

signals:
    // QRunnable interface
public:
    void run();
    bool wait(int msecs);
private:
    QByteArray          disconnectReason;
    QRandomGenerator    *randomGenerator;
    QString             generateAccountNumber();
    QString             generateToken();
    Database            *m_Db;
    bool                Connected;
    QString             Token;
    QString             userInfo;
    qintptr             handle;
    QTcpSocket          *socket;
    QByteArray          handleData(QString data);
    int                 saveUser(QStringList params);
    int                 loginUser(QStringList params);
    int                 transferMoney(QStringList userParams);
    int                 depositMoney(QStringList params);
    int                 withDrawMoney(QStringList params);
    int                 showUserInfo(QStringList params);
    void                closeConnection();

};

#endif // CLIENT_H
