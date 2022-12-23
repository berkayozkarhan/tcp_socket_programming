#include "utils.h"



QString getAddressFromSocket(QTcpSocket *socket)
{
    QString address;
    address = socket->peerAddress().toString().split(':')[3];
    return address;
}
