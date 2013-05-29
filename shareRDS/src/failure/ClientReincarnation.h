#ifndef __SHARERDS_CLIENTREINCARNATION_H_
#define __SHARERDS_CLIENTREINCARNATION_H_

#include "omnetpp.h"
#include <string>
#include "SystemMsg_m.h"

class ClientReincarnation : public cSimpleModule
{
    public:
        //First Message at initialize time to notify all
        //that i'm alive
        int clientID;
        //Message to decide when die or live
        SystemMsg *reinc;
        bool willfail;
    protected:
        void initialize();
        void handleMessage(cMessage *msg);

};

#endif
