#include "ClientApplication.h"
#include "../messages/SystemMsg_m.h"

Define_Module(ClientApplication);
void ClientApplication::initialize()
{
    this.clientID = par("clientID");
    if (clientID == -1 )
        throw cRuntimeError ("Invalid client ID %d;must be >= 0", clientID );
}

void ClientApplication::handleMessage(cMessage *msg)
{
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    if (clientID == ttmsg->clientID){
        int operation = ttmsg->getOperation();
        if ((operation == READ) && (ttmsg->FAIL)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<") ERROR no data item with id " << ttmsg->msgDataID << "\n";
        }
        else if ((operation == READ) && (ttmsg->SUCCESS)){
            //Here i have to add to my local copy the value read from the replica and save
            //it
        }
        else if ((operation == WRITE)&&(ttmsg->SUCCESS)){

        }
    }


}

int ClientReincarnation::getClientID()
{
    return this.clientID;
}
void ClientReincarnation::setClientID(String clientID)
{
    this.clientID = clientID;
}

