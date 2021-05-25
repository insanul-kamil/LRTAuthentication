#ifndef RFID_H
#define RFID_H

class RFID
{
    public:
        bool readCardUID();             // return bool if theres card detected and can be readed
        char* getUID();

    private:
        char content_[15] = "";

};


#endif