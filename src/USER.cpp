#include "USER.h"

User::User(int id, char name[15], char uid[15],int faces)
{
    ID = id;
    //NAME = name;
    strcpy(NAME, name);
    strcpy(UID, uid);
    //UID = uid;
    FACES = faces;
    

}

User::User()
{
}

int User::getId()
{
    return ID;
}

char *User::getName()
{
    return NAME;
}

char *User::getUID()
{
    return UID;

}
int User::getFaces()
{
    return FACES;
}
