#ifndef __SHARERDS_APPLICATION_H_
#define __SHARERDS_APPLICATION_H_

#include <cstdlib>
#include "SystemMsg_m.h"
using namespace std;

class Application : public  cSimpleModule
{
    public:
        SystemMsg *ttmsg;
        int clientID;
        bool is_large;
        std::map<std::string,int> ownedDataItems;
        //get and set methods
        int getClientID();
        void setClientID(int clientID);
        int getDataItem(std::string dataID);
        void setDataItem(int dataValue);
    protected:
        virtual SystemMsg *generateMessage();
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

char* itoa(int value, char* result, int base) {
        // check that the base if valid
        if (base < 2 || base > 36) { *result = '\0'; return result; }

        char* ptr = result, *ptr1 = result, tmp_char;
        int tmp_value;

        do {
            tmp_value = value;
            value /= base;
            *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
        } while ( value );

        // Apply negative sign
        if (tmp_value < 0) *ptr++ = '-';
        *ptr-- = '\0';
        while(ptr1 < ptr) {
            tmp_char = *ptr;
            *ptr--= *ptr1;
            *ptr1++ = tmp_char;
        }
        return result;
 }
#endif
