#ifndef __SHARERDS_CLIENTAPPLICATION_H_
#define __SHARERDS_CLIENTAPPLICATION_H_

#include <cstdlib>
#include <omnetpp.h>

class ClientApplication : public  cSimpleModule
{
    public:


    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};
#endif
