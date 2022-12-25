#include "bank.h"


Bank::Bank(QString bankName, QJsonObject bankInfo)
{
    isAvailable = false;
    if(!bankInfo.isEmpty()) {
        this->name = bankName;
        this->transferFee = bankInfo.take("transferfee").toDouble();
        isAvailable = true;
        return;
    }
}


