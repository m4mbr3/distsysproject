#ifndef __SHARERDS_APPLICATION_H_
#define __SHARERDS_APPLICATION_H_

#include <cstdlib>
class Application : public  cSimpleModule
{
    public:
        int clientID;
        int maxNReplicat;
        int vector<int> replicaIDs;
        std::map<std::string,int> ownedDataItems;
        //get and set methods
        virtual int getMaxNReplica();
        virtual void setMaxNReplica(int numberOfReplica);
        virtual int getClientID();
        virtual void setClientID(int clientID);
        virtual int getDataItem(std::string dataID);
        virtual void setDataItem(int dataValue);
    protected:
        virtual SystemMsg *generateMEssage();
        virtual forwardMessage(SystemMsg *ttmsg);
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};
#endif
