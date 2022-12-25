#ifndef USER_H
#define USER_H
#include <QObject>
#include <QJsonObject>

class User
{
public:
    User() { isAvailable = false; }
    explicit User(QString userName, QJsonObject userInfo);

    // getters
    bool        isAvailable;
    QJsonObject getJson();
    QString     getName()       const { return name; }
    QString     getToken()      const { return token; }
    QString     getSurname()    const { return surname; }
    double      getBalance()    const { return balance; }
    QString     getPassword()   const { return password; }
    QString     getUserName()   const { return userName; }
    QString     getAccountNo()  const { return accountNo; }
    QString     getBankName()   const { return bankName; }

    //setters
    void setName        (const QString &newName)        { name = newName; }
    void setToken       (const QString &newToken)       { token = newToken; }
    void setBalance     (double newBalance)             { balance = newBalance; }
    void setSurname     (const QString &newSurname)     { surname = newSurname; }
    void setUserName    (const QString &newUserName)    { userName = newUserName; }
    void setBankName    (const QString &newBankName)    { bankName = newBankName; }
    void setPassword    (const QString &newPassword)    { password = newPassword; }
    void setAccountNo   (const QString &newAccountNo)   { accountNo = newAccountNo; }

private:
    QString name;
    QString token;
    double  balance;
    QString surname;
    QString password;
    QString userName;
    QString bankName;
    QString accountNo;
};
#endif // USER_H
