#include "USER.h"

User::User(int id, char name_[15], char uid[15],int faces)
{
    ID = id;
    strcpy(NAME, name_);         // memory overlap happens here
    strcpy(UID, uid);           // memory overlap happens here
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

