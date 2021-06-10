#include "ACCESS.h"

extern unsigned long startMillis;
extern unsigned long currentMillis;

bool ACCESS::userExist(char *scannedUID,char *uid[4], char *bufname[4])       // check if user that scanned his card exist in database. return user name.
{
    // memset(userName, 0, sizeof(15));  this cause corruption to name bufname data
    userName = '\0';

    for(int i = 0; i < numUser; i++)
    {
        if(strcmp(scannedUID,uid[i]) == 0)
        {
            _id = i;
            Serial.print(i);
            userName = bufname[i];
            Serial.print("-User : ");
            Serial.print(userName);

            return true;
        }
    }
    return false;
}

int ACCESS::getUserID()
{
    return _id;
}

char *ACCESS::GetUserName(int id,char *bufname[])
{
    userName = '\0';
    userName = bufname[id];

    return userName;
}

char *ACCESS::getUserName()
{
    return userName;
}

void ACCESS::recvSerial()
{
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) 
        {
            if (rc != endMarker) 
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) 
                {
                    ndx = numChars - 1;
                }
            }
            else 
            {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) 
        {
                recvInProgress = true;
        }
    } 
}

char *ACCESS::getData()
{
    if (newData == true) 
    {
        Serial.print("\nData from python: ");
        Serial.println(receivedChars);
        newData = false;
    }
    return receivedChars;
}