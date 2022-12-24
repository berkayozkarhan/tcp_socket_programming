#include "client.h"
#include "globals.h"
#include <time.h>

const static char *Banner = "Welcome. Please specify operation described below. \n"
                            "signin : Sign in, usage : signin <username> <password>\n"
                            "login  : Login, usage : login <username> <password>\n"
                            "quit   : Quit, usage : quit\n";


Client::Client(qintptr handle, Database *db, QObject *parent)
    : QObject{parent}, QRunnable{}
{
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

    QByteArray res = Banner;
    while(socket->write(res), socket->waitForBytesWritten(), socket->waitForReadyRead(10000)) {
        QString rcvString = "";
        while(!rcvString.endsWith("\r\n")) {
            rcvString += QString(socket->read(1));
        }
        qDebug() << "client : " << rcvString;
        res = handleData(rcvString);
        /*
        int opCode = socket->read(3)[0];
        qDebug() << "operation : " << opCode - '0';
        if(opCode -'0' < 0) {
            qDebug() << "wrong operation.";
            break;
        }
        socket->flush();
        */

        //QByteArray rcvData = socket->readAll();
        //TO:DO : emit dataReceived(rcvData);
        //qInfo() << "Data : " << rcvData;
        //res = "server_response";
    }

    res = "timeout";
    socket->write(res);
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
            res = "TOKEN=" + Token;
            break;
        case G::Result::WrongParameters:
            res = "WRONG_PARAMETERS";
            break;
        default:
            res = "UNKNOWN";
            break;
        }
    }
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
    QString password = params[2];

    Token = "tokenTest123";

    return G::Result::LoginSuccessful;
}

