#include "USER.h"

User::User(int id, char name, char uid,int faces)
{
    ID = id;
    NAME = name;
    UID = UID;
    FACES = faces;

}

User::User()
{
}

int User::getId()
{
    return ID;
}

char User::getName()
{
    return NAME;
}

char User::getUID()
{
    return UID;

}
int User::getFaces()
{
    return FACES;
}