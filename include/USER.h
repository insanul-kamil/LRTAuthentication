#ifndef USER_H
#define USER_H

#include <Arduino.h>

class User
{
    public:
        User(int id, char name[15], char uid[15],int faces);
        User();
        int getId();
        char *getName();
        char *getUID();
        int getFaces();
        
    private:
        int ID;
        char name[15];
        char NAME[15];
        char UID[15];
        int FACES;
};


#endif