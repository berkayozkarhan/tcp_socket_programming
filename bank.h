#ifndef BANK_H
#define BANK_H

#include <QObject>
#include <QJsonObject>

class Bank
{
public:
    explicit Bank(QString bankName, QJsonObject bankInfo);
    bool    isAvailable;
    double  getTransferFee()    const { return transferFee; }
    QString getName()           const { return name; }
private:
    QString name;
    double transferFee;

};

#endif // BANK_H
