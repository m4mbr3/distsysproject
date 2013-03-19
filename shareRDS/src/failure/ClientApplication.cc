#include "ClientApplication.h"
#include "../messages/SystemMsg_m.h"

Define_Module(ClientApplication);
void ClientApplication::initialize()
{
    this.clientID = par("clientID");
    this.maxNReplica = par("maxNReplica");
    if (clientID == -1 )
        throw cRuntimeError ("Invalid client ID %d;must be >= 0", clientID );
    SystemMsg *ttmsg = new SystemMsg();
    scheduleAt(3.0, ttmsg);

}

void ClientApplication::handleMessage(cMessage *msg)
{
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    if ((ttmsg->getClientID()== -1)
            &&(ttmsg->getReplicaID()== -1)
                &&(ttmsg->getLamportClock()== -1)
                    &&(ttmsg->getReplicaCode() == -1)
                        &&(ttmsg->getOperation()== -1)){
        // I received a message from myself
        scheduleAt(4.0, ttmsg);
        SystemMsg msgGen = ClientApplication::generateMessage();
        send (msgGen, "out");
    }
    else if (clientID == ttmsg->clientID){
        int operation = ttmsg->getOperation();
        int isSuccess = ttmsg->getReplicaCode();
        if ((operation == READ) && (!ttmsg->isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<") ERROR no data item with id " << ttmsg->msgDataID << "\n";
        }
        //to check if operation_var can be equal to SUCCESS
        else if ((operation == READ) && (ttmsg->isSuccess)){
            //Here I have to add to my local copy the value read from the replica and saved
            //item
         /*   if (ownedDataItems.at(ttmsg->getDataID())== std::result_out_of_range){
                ownedDataItems[ttmsg->getDataID()] = ttmsg->getData();
            }*/
            ownedDataItems[ttmsg->getDataID()]=ttmsg->getData();
        }
        else if ((operation == WRITE)&&(!ttmsg->isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<")ERROR fail  write our datavalue at replica "<< ttmsg->replicaID <<"\n";
        }
        else if ((operation == WRITE) && (ttmsg->isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<")NOTIFICATION data written correctly at replica "<< ttmsg->replicaID <<"\n";
        }
        delete ttmsg;
    }
}

SystemMsg* ClientApplication::generateMessage(){
    //This is the function that generates random messages to replica

    SystemMsg *ttmsg = new SystemMsg();
    ttmsg->setClientId(this.clientID);
    ttmsg->setReplicaID(intuniform(0, maxNReplica));
    ttmsg->setOperation(intuniform(0,1));
    ttmsg->setData(intuniform(-1000, 1000));
    ttmsg->setDataID((intuniform(0,100)%2==0): (intuniform(0,25)+'a') ? (intuniform(0,25)+'A'));

    return ttmsg;
}
int ClientApplication::getMaxNReplica(){
    return maxNReplica;
}
void ClientApplication::setMaxNReplica(int numberOfReplica){
    this.maxNReplica =numberOfReplica;
}
int ClientApplication::getClientID()
{
    return this.clientID;
}
void ClientApplication::setClientID(String clientID)
{
    this.clientID = clientID;
}

