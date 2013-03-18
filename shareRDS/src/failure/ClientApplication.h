#ifndef __SHARERDS_CLIENTAPPLICATION_H_
#define __SHARERDS_CLIENTAPPLICATION_H_

#include <cstdlib>
class ClientApplication : public  cSimpleModule
{
    public:
        int clientID;
        std::map<std::string,int> ownedDataItems;
        FailureManager fm;
        ApplicationManager am;
        //get and set methods
        virtual int getClientID();
        virtual void setClientID(int clientID);
        virtual int getDataItem(std::string dataID);
        virtual void setDataItem(int dataValue);
        //Class costructor
        virtual ClientReincarnation(String clientID);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};
#endif
