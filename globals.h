#ifndef GLOBALS_H
#define GLOBALS_H

namespace G {

namespace Server {
    const static int port = 5000;
}

namespace Result {
    const static int UserExists = 1;
    const static int SaveUserSuccessful = 2;
    const static int WrongParameters = 3;
    const static int LoginSuccessful = 4;
    const static int RegistrationFailed = 5;
    const static int WrongPassword = 6;
    const static int UserDoesNotExist = 7;
    const static int InvalidToken = 8;
    const static int InsufficientBalance = 9;
    const static int InvalidAccountNo = 10;
    const static int TransferSuccessful = 11;
    const static int DepositSuccessful = 12;
}

namespace Db {
    //const static char* type = "json";
    //const static char *version = "0.0.1";
    const static char *filePath = "/home/berkay/Desktop/accounts_db.json";

    namespace Result {
        const int ErrDbFileInitialize = 102;
        const int ErrDbDocumentInitialize = 101;
        const int DbOpenSuccessful = 100;
        const int UserAlreadyExists = 200;
        const int PushSuccessful = 201;
        const int PushFailed = 202;
    }

}

namespace App {
    const static int TimeoutMs = 10000 * 10; //
}



}

#endif // GLOBALS_H
