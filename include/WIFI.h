#ifndef WIFI_H
#define WIFI_H

class WIFI
{
    public:
        void initWifi();            // connect to wifi
        void reconnect();           // loop to reconnect to mqtt broker if disconnected

    private:
        char UID;
};


#endif