#ifndef BANK_H
#define BANK_H

#include <QObject>
#include <QJsonObject>

class Bank
{
public:
    explicit Bank(QString bankName, QJsonObject bankInfo);
    bool    isAvailable;
    int     getTransferFee()    const { return transferFee; }
    QString getName()           const { return name; }
private:
    QString name;
    int transferFee;

};

#endif // BANK_H
