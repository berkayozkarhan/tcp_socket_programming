#include "database.h"
//#include "globals.h"
#include <QJsonParseError>
#include <QJsonArray>
#include "globals.h"

Database::Database(QString filePath, QObject *parent)
    : QObject{parent}
{
    isOpen = false;
    this->m_filePath = filePath;
    qDebug() << "file path : " << this->m_filePath;
}

int Database::open()
{
    if(!initDbFile()) { qDebug() << "db file initialize error."; return G::Db::Result::ErrDbFileInitialize; }
    qDebug() << "Successfully initialized db file.";

    if(!initDocument()) { qDebug() << "db document initialize error."; return G::Db::Result::ErrDbDocumentInitialize; }
    qDebug() << "Successfully initialized db document.";
    isOpen = true;
    return G::Db::Result::DbOpenSuccessful;
}

int Database::initDbFile()
{
    m_dbFilePtr = new QFile(this->m_filePath);
    if(!m_dbFilePtr->open(QIODevice::ReadWrite)) {
        return 0;
    }
    return 1;
}

int Database::initDocument()
{
    QByteArray bytes = m_dbFilePtr->readAll();
    m_dbFilePtr->close();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);

    if(!document.isObject()) { return 0; }

    this->m_Document = document.object();
    return 1;
}

QJsonValueRef Database::getUsers()
{
    return this->m_Document["users"];
}

QList<User> Database::getUserList()
{
    QJsonObject users = getUsers().toObject();
    QList<User> userList;
    foreach (QString userName, users.keys()) {
        QJsonObject userInfo = getUserInfo(userName);
        User user(userName, userInfo);
        userList.append(user);
    }
    return userList;
}

QJsonValueRef Database::getBanks()
{
    return this->m_Document["banks"];
}

QList<Bank> Database::getBankList()
{
    QJsonObject banks = getBanks().toObject();
    QList<Bank> bankList;
    foreach (QString bankName, banks.keys()) {
        QJsonObject bankInfo = getBankInfo(bankName);
        Bank bank(bankName, bankInfo);
        bankList.append(bank);
    }
    return bankList;
}

QJsonObject Database::getUserInfo(QString userName)
{
    QJsonObject users = getUsers().toObject();
    return users.take(userName).toObject();
}

QJsonObject Database::getBankInfo(QString bankName)
{
    QJsonObject banks = getBanks().toObject();
    return banks.take(bankName).toObject();
}

Bank Database::getBank(QString bankName)
{
    QJsonObject bankInfo = getBankInfo(bankName);
    Bank bank(bankName, bankInfo);
    return bank;
    //return bankInfo.isEmpty() ? Bank("unknown", bankInfo) : Bank(bankName, bankInfo);
}

User Database::getUserByAccountNo(QString accountNo)
{
    QList userList = getUserList();
    User user;
    foreach (User u, userList) {
        if(!QString::compare(accountNo, u.getAccountNo()))
            return u;
    }
    return user; // returns empty user.
}

User Database::getUser(QString userName)
{
    QJsonObject userInfo = getUserInfo(userName);
    User user(userName, userInfo);
    return user;
    //return userInfo.isEmpty() ? User("unknown", userInfo) : User(userName, userInfo);
}

int Database::updateUser(User user)
{
    QJsonObject users = getUsers().toObject();
    QJsonObject userUpdate = users.take(user.getUserName()).toObject();
    userUpdate.insert("account_no", user.getAccountNo());
    userUpdate.insert("name", user.getName());
    userUpdate.insert("surname", user.getSurname());
    userUpdate.insert("balance", user.getBalance());
    userUpdate.insert("token", user.getToken());
    userUpdate.insert("password", user.getPassword());
    users[user.getUserName()] = userUpdate;
    (void)commit("users", users);
    return pushLastCommit();
}

int Database::updateUserBalance(QString userName, int newBalance)
{
    QJsonObject users = getUsers().toObject();
    QJsonObject userUpdated = users.take(userName).toObject();
    userUpdated.insert("balance", newBalance);
    users[userName] = userUpdated;
    (void)commit("users", users);
    return pushLastCommit();


    //getUsers().toObject().take(userName).toObject().insert("balance", newBalance);
    //return 0;
}

int Database::updateUserBank(QString userName, QString newBankName)
{
    QJsonObject users = getUsers().toObject();
    QJsonObject userUpdated = users.take(userName).toObject();
    userUpdated.insert("bank", newBankName);
    users[userName] = userUpdated;
    (void)commit("users", users);
    return pushLastCommit();

    //getUsers().toObject().take(userName).toObject().insert("bank", newBankName);
    //return 0;
}

int Database::updateUserToken(QString userName, QString newToken)
{
    QJsonObject users = getUsers().toObject();
    QJsonObject userUpdated = users.take(userName).toObject();
    userUpdated.insert("token", newToken);
    users[userName] = userUpdated;
    (void)commit("users", users);
    return pushLastCommit();

    //getUsers().toObject().take(userName).toObject().insert("token", newToken);
    //return 0;
}

int Database::registerNewUser(User user)
{
    QJsonObject users = getUsers().toObject();
    if(users.keys().contains(user.getUserName())) {
        qDebug() << "User already exists.";
        return G::Db::Result::UserAlreadyExists;
    }

    QJsonObject userUpdate; // = users.take(user.getUserName()).toObject();
    userUpdate.insert("account_no", user.getAccountNo());
    userUpdate.insert("name", user.getName());
    userUpdate.insert("surname", user.getSurname());
    userUpdate.insert("balance", user.getBalance());
    userUpdate.insert("token", user.getToken());
    userUpdate.insert("bank", user.getBankName());
    userUpdate.insert("password", user.getPassword());
    users[user.getUserName()] = userUpdate;
    (void)commit("users", users);

    return pushLastCommit();
}

int Database::pushLastCommit()
{
    QJsonObject objPush = this->m_lastCommit;
    QJsonDocument updatedDoc(objPush);
    if(!m_dbFilePtr->isOpen())
        m_dbFilePtr->open(QIODevice::ReadWrite);
    m_dbFilePtr->resize(0);
    m_dbFilePtr->write(updatedDoc.toJson());
    m_dbFilePtr->close();
    return G::Db::Result::PushSuccessful;
}

int Database::commit(QString key, QJsonObject chg)
{
    this->m_lastCommit = this->m_Document;
    this->m_lastCommit[key] = chg;
    return 1;
}

