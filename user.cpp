#include "user.h"
#include "bank.h"

User::User(QString userName, QJsonObject userInfo)
{
    isAvailable = false;
    if(!userInfo.isEmpty()) {
        this->userName = userName;
        this->accountNo = userInfo.take("account_no").toString();
        this->name = userInfo.take("name").toString();
        this->surname = userInfo.take("surname").toString();
        this->bankName = userInfo.take("bank").toString();
        this->balance = userInfo.take("balance").toInt();
        this->token = userInfo.take("token").toString();
        isAvailable = true;
        return;
    }
}

QJsonObject User::getJson()
{
    QJsonObject json;
    json["account_no"] = this->accountNo;
    json["name"] = this->name;
    json["surname"] = this->surname;
    json["balance"] = this->balance;
    json["bank"] = this->bankName;
    json["token"] = this->token;
    return json;
}

















