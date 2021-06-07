#ifndef ACCESS_H
#define ACCESS_h

#include <Arduino.h>
#include "USER.h"

class ACCESS
{
    public:
        // check if user that scanned his card exist in database. return user name.
        bool userExist(char *scannedUID,char *uid[4],char *bufname[4]);
        // return user name that have been detected
        char *getUserName();
        // get the user name that have been detected in face rec
        char *GetUserName(int id, char *bufname[]);
        // read serial with start with certain start key
        void recvSerial();
        // get data that have been parsed from recvSerial()
        char *getData();
    private:
        int numUser = 4;
        char *userName;

        const byte numChars = 32;
        char receivedChars[32];

        boolean newData = false;

};


#endif