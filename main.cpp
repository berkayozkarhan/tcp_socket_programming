#include <QCoreApplication>
#include <QDebug>
#include "server.h"
#include "globals.h"
#include "database.h"
#include "bank.h"
#include "user.h"
/*
 * :: API ::
:/register :
    @param name : str
    @param surname : str
    @param identity_no: str (pass)
    @param username: str
    @param password: str
    @param token : str

:/transfer
    token: str
    account_number: str



*/


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Database *db = new Database(G::Db::filePath);
    db->open();
    /*
    QList<User> userList = db->getUserList();
    QList<Bank> bankList = db->getBankList();
    Bank bank = db->getBank("Bank3");
    User user = db->getUser("berkay");
    User user1;
    user1.setUserName("test_username");
    user1.setAccountNo("test-account-no");
    user1.setBalance(100);
    user1.setBankName("test-bank");
    user1.setToken("test-token");
    //db->registerNewUser(user1);
    //user.setBalance(3456);
    //db->updateUser(user);
    //db->updateUserBalance("berkay", 7654);
    //db->updateUserBank("berkay", "Bank3");
    //db->updateUserToken("berkay", "new_token_123123");
    */

    /*
    User user = db->getUserByAccountNo("asdasd");
    if(user.isAvailable)
        qDebug() << "name : " << user.getUserName();
    */

    Server server;
    server.start(G::Server::port);
    return a.exec();
}
