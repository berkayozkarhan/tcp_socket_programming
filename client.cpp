#include "client.h"

Client::Client(qintptr handle, QObject *parent)
    : QObject{parent}, QRunnable{}
{
    this->handle = handle;
}

void Client::run()
{
    qInfo() << this << " run " << QThread::currentThread();
    socket = new QTcpSocket(nullptr);
    if(!socket->setSocketDescriptor(handle)) {
        //TO:DO : emit Er
        qCritical() << socket->errorString();
        delete socket;
        return;
    }

    socket->write(Banner);

    while(socket->waitForReadyRead(20000)) {
        QByteArray rcvData = socket->readAll();
        //TO:DO : emit dataReceived(rcvData);
        qInfo() << "Data : " << rcvData;

        QByteArray response("server_Data");
        socket->write(response);
        socket->waitForBytesWritten();
    }

    socket->close();
    socket->deleteLater();
    qInfo() << this << " done " << QThread::currentThread();

}

bool Client::wait(int msecs)
{
    return socket->waitForReadyRead(msecs);
}

