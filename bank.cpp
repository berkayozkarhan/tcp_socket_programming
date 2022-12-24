#include "bank.h"


Bank::Bank(QString bankName, QJsonObject bankInfo)
{
    isAvailable = false;
    if(!bankInfo.isEmpty()) {
        this->name = bankName;
        this->transferFee = bankInfo.take("transferfee").toInt();
        isAvailable = true;
        return;
    }
}


