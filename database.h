#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QJsonObject>
#include "user.h"
#include "bank.h"
class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QString filePath, QObject *parent = nullptr);
    User        getUser(QString userName);
    Bank        getBank(QString bankName);
    User        getUserByAccountNo(QString accountNo);
    QList<User> getUserList();
    QList<Bank> getBankList();
    QJsonObject getUserInfo(QString userName);
    QJsonObject getBankInfo(QString bankName);

    int         updateUser(User user);
    int         updateUserBalance(QString userName, double newBalance);
    int         updateUserBank(QString userName, QString newBankName);
    int         updateUserToken(QString userName, QString newToken);
    int         registerNewUser(User user);
    int         open();
    bool        isOpen;

signals:

private:
    QJsonObject     m_currentDocument;
    QJsonObject     m_lastCommit;
    QString         m_filePath;
    int             nextAccountNo;
    QFile           *m_dbFilePtr;
    QJsonValueRef   getBanks();
    QJsonValueRef   getUsers();
    int             initDbFile();
    int             initDocument();
    int             commit(QString key, QJsonObject chg);
    int             pushLastCommit();
};

#endif // DATABASE_H
