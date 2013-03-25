#ifndef __SHARERDS_CLIENTREINCARNATION_H_
#define __SHARERDS_CLIENTREINCARNATION_H_

#include <omnetpp.h>
#include <string>

class ClientReincarnation : public cSimpleModule
{
    public:
        SystemMsg* ttmsg;
        int clientID;
    protected:
        void initialize();
        void handleMessage(cMessage *msg);

};

#endif
