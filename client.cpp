#include "client.h"
#include "globals.h"
#include <time.h>

const static char *Banner = "Welcome. Please specify operation described below. \n"
                            "signin   : Sign in, usage : signin <username> <password>\n"
                            "login    : Login, usage : login <username> <password>\n"
                            "transfer : Transfer money, usage : transfer <(str)username> <(str)token> <(str)target_account_no> <(double)amount>\n"
                            "For transfer, first login and get token. After you got token, use this token to transfer money.\n"
                            "quit   : Quit, usage : quit\n";


QByteArray Banner1 = "Welcome. Please specify operation described below. \n"
                      "signin   : Sign in, usage : signin <username> <password>\n"
                      "login    : Login, usage : login <username> <password>\n"
                      "transfer : Transfer money, usage : transfer <(str)username> <(str)token> <(str)target_account_no> <(double)amount>\n"
                      "For transfer, first login and get token. After you got token, use this token to transfer money.\n"
                      "quit   : Quit, usage : quit\n";

Client::Client(qintptr handle, Database *db, QObject *parent)
    : QObject{parent}, QRunnable{}
{
    this->disconnectReason = "timeout";
    this->handle = handle;
    this->m_Db = db;
    randomGenerator = new QRandomGenerator(time(NULL));
}

void Client::run()
{
    qInfo() << this << " run " << QThread::currentThread();

    if(!m_Db->isOpen)
        m_Db->open();

    socket = new QTcpSocket(nullptr);
    if(!socket->setSocketDescriptor(handle)) {
        qCritical() << socket->errorString();
        delete socket;
        return;
    }

    QByteArray res = Banner1;
    while(socket->write(res), socket->waitForBytesWritten(), socket->waitForReadyRead(G::App::TimeoutMs)) { // 60 seconds
        QString rcvString = "";
        while(!rcvString.endsWith("\r\n")) {
            rcvString += QString(socket->read(1));
        }
        qDebug() << "client : " << rcvString;
        res = handleData(rcvString);
        if (!QString::compare("UNKNOWN", res)) {
            this->disconnectReason = "UNKNOWN_COMMAND";
            break;
        }
    }
    closeConnection();
    exit(0);
}

void Client::closeConnection()
{
    socket->write(this->disconnectReason);
    socket->waitForBytesWritten();
    socket->close();
    socket->deleteLater();
    qInfo() << this << " done " << QThread::currentThread();
}

bool Client::wait(int msecs)
{
    return socket->waitForReadyRead(msecs);
}

QString Client::generateAccountNumber()
{
    int number = randomGenerator->bounded(100, 999);
    return QString::number(number);
}

QString Client::generateToken()
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 12; // assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = randomGenerator->global()->generate() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;


    int token = randomGenerator->bounded(10000, 100000);
    return QString::number(token);
}

QByteArray Client::handleData(QString data)
{
    QString res = "UNKNOWN";
    QString operation = data.split(' ').at(0);

    if(!QString::compare(operation, "signin")) {
        QStringList params = data.split(' ');
        int saveResult = saveUser(params);
        switch (saveResult) {
        case G::Result::SaveUserSuccessful:
            res = "REGISTRATION_SUCCESSFUL";
            break;
        case G::Result::UserExists:
            res = "USER_ALREADY_EXISTS";
            break;
        case G::Result::WrongParameters:
            res = "WRONG_PARAMETERS";
            break;
        default:
            res = "UNKNOWN";
            break;
        }
    }else if(!QString::compare(operation, "login")) {
        QStringList params = data.split(' ');
        int loginResult = loginUser(params);
        switch (loginResult) {
        case G::Result::LoginSuccessful:
            res = "LOGIN_SUCCESSFUL.TOKEN=" + Token;
            break;
        case G::Result::WrongParameters:
            res = "WRONG_PARAMETERS";
            break;
        case G::Result::UserDoesNotExist:
            res = "USER_DOES_NOT_EXIST";
            break;
        default:
            res = "UNKNOWN";
            break;
        }
    }
    else if(!QString::compare(operation, "transfer")) {
        QStringList params = data.split(' ');
        //transfer  berkay  new_token_123123 123456 10
        int transferResult = transferMoney(params);
        switch (transferResult) {
        case G::Result::WrongParameters:
            res = "WRONG_PARAMETERS";
            break;
        case G::Result::UserDoesNotExist:
            res = "USER_DOES_NOT_EXIST";
            break;
        case G::Result::InvalidToken:
            res = "INVALID_TOKEN";
            break;
        case G::Result::InvalidAccountNo:
            res = "INVALID_ACCOUNT_NO";
            break;
        case G::Result::InsufficientBalance:
            res = "INSUFFICIENT_BALANCE";
            break;
        case G::Result::TransferSuccessful:
            res = "TRANSFER_OK.TOKEN=" + this->Token;
            break;
        default:
            res = "UNKNOWN";
            break;
        }
    } else if(!QString::compare(operation, "deposit")) {
        QStringList params = data.split(' ');
        int depositResult = depositMoney(params);
        switch (depositResult) {
        case G::Result::WrongParameters:
            res = "WRONG_PARAMETERS";
            break;
        case G::Result::UserDoesNotExist:
            res = "USER_DOES_NOT_EXIST";
            break;
        case G::Result::InvalidToken:
            res = "INVALID_TOKEN";
            break;
        case G::Result::DepositSuccessful:
            res = "DEPOSIT_SUCCESSFUL.TOKEN=" + this->Token;
            break;
        default:
            res = "UNKNOWN";
            break;
        }
    } else if(!QString::compare(operation, "show")) {
        QStringList params = data.split(' ');
        int showResult = showUserInfo(params);
        switch (showResult) {
        case G::Result::WrongParameters:
            res = "WRONG_PARAMETERS";
            break;
        case G::Result::UserDoesNotExist:
            res = "USER_DOES_NOT_EXIST";
            break;
        case G::Result::InvalidToken:
            res = "INVALID_TOKEN";
            break;
        case G::Result::ShowUserSuccessful:
            res = this->userInfo + "TOKEN=" + this->Token;
            break;
        default:
            res = "UNKNOWN";
            break;
        }
    } else if(!QString::compare(operation, "wdraw")) {
        QStringList params = data.split(' ');
        int wdrawResult = withDrawMoney(params);
        switch (wdrawResult) {
        case G::Result::WrongParameters:
            res = "WRONG_PARAMETERS";
            break;
        case G::Result::UserDoesNotExist:
            res = "USER_DOES_NOT_EXIST";
            break;
        case G::Result::InvalidToken:
            res = "INVALID_TOKEN";
            break;
        case G::Result::InsufficientBalance:
            res = "INSUFFICIENT_BALANCE";
            break;
        case G::Result::WithDrawSuccessful:
            res = "WIDTH_DRAW_SUCCESSFUL.TOKEN=" + this->Token;
            break;
        default:
            res = "UNKNOWN";
            break;
        }
    }
    else
        res = "UNKNOWN";
    return res.toUtf8();
}

int Client::saveUser(QStringList params)
{
    if(params.length() != 3) {
        qDebug() << "wrong parameters.";
        return G::Result::WrongParameters;
    }

    QString userName = params[1];
    QString password = params[2].trimmed().replace(QRegularExpression("[^a-zA-Z0-9\\s]"), "");

    User newUser;
    newUser.setUserName(userName);
    newUser.setPassword(password);
    newUser.setToken(generateToken());
    newUser.setAccountNo(generateAccountNumber());
    newUser.setBankName("Bank1");
    newUser.setBalance(0);

    int res = m_Db->registerNewUser(newUser);

    if( res == G::Db::Result::PushSuccessful)
        return G::Result::SaveUserSuccessful;
    if(res == G::Db::Result::UserAlreadyExists)
        return G::Result::UserExists;
    return G::Result::RegistrationFailed;
}

int Client::loginUser(QStringList params)
{
    //login <username> <password>
    if(params.length() != 3) {
        qDebug() << "wrong parameters.";
        return G::Result::WrongParameters;
    }

    QString userName = params[1];
    QString password = params[2].trimmed().replace(QRegularExpression("[^a-zA-Z0-9\\s]"), "");;

    User user = m_Db->getUser(userName);
    if(!user.isAvailable)
        return G::Result::UserDoesNotExist;

    if(!QString::compare(password, user.getPassword())) {
        QString newToken = generateToken();
        m_Db->updateUserToken(user.getUserName(), newToken);
        this->Token = newToken;
        return G::Result::LoginSuccessful;
    }

    return G::Result::WrongPassword;
}

int Client::transferMoney(QStringList userParams)
{
    //transfer <username> <token> <target_account_no> <amount>
    if(userParams.length() != 5)
        return G::Result::WrongParameters;

    QString userName =userParams[1], token = userParams[2], targetAccountNo = userParams[3];
    QString strAmount = userParams[4].trimmed().replace(QRegularExpression("[^0-9\\s]"), "");
    double amount = strAmount.toDouble();
    User user = m_Db->getUser(userName);

    if(!user.isAvailable)
        return G::Result::UserDoesNotExist;

    if(QString::compare(token, user.getToken()))
        return G::Result::InvalidToken;

    //check account no is valid
    User targetUser = m_Db->getUserByAccountNo(targetAccountNo);
    if(!targetUser.isAvailable)
        return G::Result::InvalidAccountNo;

    // all params are valid
    Bank sourceBank = m_Db->getBank(user.getBankName()), targetBank = m_Db->getBank(targetUser.getBankName());
    double transferFee = 0.;
    if(QString::compare(sourceBank.getName(), targetBank.getName()))
        transferFee = sourceBank.getTransferFee();

    double userBalance = user.getBalance();

    //TO:DO : control balances.
    if(userBalance < amount)
        return G::Result::InsufficientBalance;

    double sourceBalance = userBalance - amount - transferFee;
    double targetBalance = targetUser.getBalance() + amount;
    m_Db->updateUserBalance(userName, sourceBalance);
    m_Db->updateUserBalance(targetUser.getUserName(), targetBalance);

    QString newToken = generateToken();
    m_Db->updateUserToken(userName, newToken);
    this->Token = newToken;
    return G::Result::TransferSuccessful;
}

int Client::depositMoney(QStringList params)
{
    //deposit <username> <token> <amount>
    if(params.length() != 4)
        return G::Result::WrongParameters;

    QString userName = params[1], token=params[2], strAmount = params[3];
    double dAmount = strAmount.toDouble();

    User user = m_Db->getUser(userName);
    if(!user.isAvailable)
        return G::Result::UserDoesNotExist;

    if(QString::compare(token, user.getToken()))
        return G::Result::InvalidToken;

    double amountUpdated = user.getBalance() + dAmount;
    m_Db->updateUserBalance(userName, amountUpdated);

    QString newToken = generateToken();
    m_Db->updateUserToken(userName, newToken);
    this->Token = newToken;
    return  G::Result::DepositSuccessful;
}

int Client::withDrawMoney(QStringList params)
{
    //wdraw <username> <token> <amount>
    if(params.length() != 4)
        return G::Result::WrongParameters;

    QString userName = params[1], token = params[2], strAmount = params[3].trimmed().replace(QRegularExpression("[^0-9\\s]"), "");
    double dAmount = strAmount.toDouble();
    User user = m_Db->getUser(userName);
    if(!user.isAvailable)
        return G::Result::UserDoesNotExist;

    if(QString::compare(token, user.getToken()))
        return G::Result::InvalidToken;

    double userBalance = user.getBalance();
    if(userBalance < dAmount)
        return G::Result::InsufficientBalance;

    double userBalanceUpdated = userBalance - dAmount;
    m_Db->updateUserBalance(userName, userBalanceUpdated);
    QString newToken = generateToken();
    this->Token = newToken;
    m_Db->updateUserToken(userName, newToken);
    return G::Result::WithDrawSuccessful;
}

int Client::showUserInfo(QStringList params)
{
    //show <username> <token>
    if(params.length() != 3)
        return G::Result::WrongParameters;
    QString userName = params[1], token = params[2].trimmed().replace(QRegularExpression("[^a-zA-Z0-9\\s]"), "");
    User user = m_Db->getUser(userName);
    if(!user.isAvailable)
        return G::Result::UserDoesNotExist;

    if(QString::compare(token, user.getToken()))
        return G::Result::InvalidToken;

    QString userInfo = user.getUserInfo();
    QString newToken = generateToken();
    m_Db->updateUserToken(userName, newToken);
    this->Token = newToken;
    this->userInfo = userInfo;
    return G::Result::ShowUserSuccessful;
}




