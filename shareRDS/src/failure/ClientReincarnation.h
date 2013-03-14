#ifndef __SHARERDS_CLIENTREINCARNATION_H_
#define __SHARERDS_CLIENTREINCARNATION_H_

#include <omnetpp.h>
#include <string>
#include "FailureManager.h"
#include "../replication/DataItemsManager.h"

/**
 * TODO - Generated class
 */
class ClientReincarnation : public cSimpleModule
{
    FailureManager *fmi;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    public:
        int clientID;
        std::map<std::string,int> ownedDataItems;

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
