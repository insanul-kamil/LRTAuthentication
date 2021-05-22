#ifndef RFID_H
#define RFID_H

class RFID
{
    public:
        void detectCard();
        char getUIDTag();
        bool isAuthorized();
    private:
        bool stat;
        char UID[15];


};


#endif