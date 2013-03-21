#ifndef __SHARERDS_APPLICATION_H_
#define __SHARERDS_APPLICATION_H_

#include <cstdlib>
#include <SystemMsg_m.h>
using namespace std;

class Application : public  cSimpleModule
{
    public:
        int clientID;
        int maxNReplica;
        std::map<std::string,int> ownedDataItems;
        //get and set methods
        int getMaxNReplica();
        void setMaxNReplica(int numberOfReplica);
        int getClientID();
        void setClientID(int clientID);
        int getDataItem(std::string dataID);
        void setDataItem(int dataValue);
    protected:
        virtual SystemMsg *generateMessage();
        virtual void forwardMessage(SystemMsg *ttmsg);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};
#endif
