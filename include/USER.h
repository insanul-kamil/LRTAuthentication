#ifndef USER_H
#define USER_H

class User
{
    public:
        User(int id, char name, char uid,int faces);
        User();

        int getId();
        char getName();
        char getUID();
        int getFaces();
    private:
        int ID;
        char NAME;
        char UID;
        int FACES;
};


#endif