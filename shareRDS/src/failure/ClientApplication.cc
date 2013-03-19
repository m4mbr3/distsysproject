#include "ClientApplication.h"
#include "../messages/SystemMsg_m.h"

Define_Module(ClientApplication);
void ClientApplication::initialize()
{
    this.clientID = par("clientID");

    if (clientID == -1 )
        throw cRuntimeError ("Invalid client ID %d;must be >= 0", clientID );
    SystemMsg *ttmsg = ClientApplication::generateMessage();
    scheduleAt(simTime(), ttmsg);
}

void ClientApplication::handleMessage(cMessage *msg)
{
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    if (clientID == ttmsg->clientID){
        int operation = ttmsg->getOperation();
        int isSuccess = ttmsg->replyCode_var;
        if ((operation == READ) && (!ttmsg->isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<") ERROR no data item with id " << ttmsg->msgDataID << "\n";
        }
        //to check if operation_var can be equal to SUCCESS
        else if ((operation == READ) && (ttmsg->isSuccess)){
            //Here I have to add to my local copy the value read from the replica and save
            //item
         /*   if (ownedDataItems.at(ttmsg->getDataID())== std::result_out_of_range){
                ownedDataItems[ttmsg->getDataID()] = ttmsg->getData();
            }*/
            ownedDataItems[ttmsg->getDataID()]=ttmsg->getData();
        }
        else if ((operation == WRITE)&&(!ttmsg->isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<")ERROR fail to write our datavalue at replica "<< ttmsg->replicaID <<"\n";
        }
        else if ((operation == WRITE) && (ttmsg->isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<")NOTIFICATION data written correctly at replica "<< ttmsg->replicaID <<"\n";
        }
    }

    delete ttmsg;
}

SystemMsg* ClientApplication::generateMessage(){
    //This is the function that generates random messages to replica

    SystemMsg *ttmsg = new SystemMsg();

    do{
        ttmsg->replicaID =
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

