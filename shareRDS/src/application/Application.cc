#include "Application.h"
#include "SystemMsg_m.h"

Define_Module(Application);
void Application::initialize()
{
    setClientID(par("clientID"));
    setMaxNReplica(par("maxNReplica"));
    if (clientID == -1 )
        throw cRuntimeError ("Invalid client ID %d;must be >= 0", clientID );
    SystemMsg *ttmsg = new SystemMsg();
    scheduleAt(3.0, ttmsg);
}

void Application::handleMessage(cMessage *msg)
{
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    if ((ttmsg->getClientID()== -1)
            &&(ttmsg->getReplicaID()== -1)
                &&(ttmsg->getLamportClock()== -1)
                    &&(ttmsg->getReplyCode() == -1)
                        &&(ttmsg->getOperation()== -1)){
        // I received a message from myself
        scheduleAt(4.0, ttmsg);
        SystemMsg *msgGen = Application::generateMessage();
        send (msgGen, "out");
    }
    else if (clientID == ttmsg->getClientID()){
        int operation = ttmsg->getOperation();
        int isSuccess = ttmsg->getReplyCode();
        if ((operation == READ) && (!isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<") ERROR no data item with id " << ttmsg->getDataID() << "\n";
        }
        //to check if operation_var can be equal to SUCCESS
        else if ((operation == READ) && (isSuccess)){
            //Here I have to add to my local copy the value read from the replica and saved
            //item
         /*   if (ownedDataItems.at(ttmsg->getDataID())== std::result_out_of_range){
                ownedDataItems[ttmsg->getDataID()] = ttmsg->getData();
            }*/
            ownedDataItems[ttmsg->getDataID()]=ttmsg->getData();
        }
        else if ((operation == WRITE)&&(!isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<")ERROR fail  write our datavalue at replica "<< ttmsg->getReplicaID() <<"\n";
        }
        else if ((operation == WRITE) && (isSuccess)){
            EV<<"CLIENT_APPLICATION: ("<< clientID <<")NOTIFICATION data written correctly at replica "<< ttmsg->getReplicaID() <<"\n";
        }
        delete ttmsg;
    }
}
void forwardMessage(SystemMsg * ttmsg){}
SystemMsg* Application::generateMessage(){
    //This is the function that generates random messages to replica

    SystemMsg *ttmsg = new SystemMsg();
    ttmsg->setClientID(clientID);
    ttmsg->setReplicaID(intuniform(0, maxNReplica));
    ttmsg->setOperation(intuniform(0,1));
    ttmsg->setData(intuniform(-1000, 1000));
    ttmsg->setDataID(((intuniform(0,100)%2)==0) ?(const char *) 'a' + rand() % (('z'-'a') + 1): (const char *)'A' + rand() % (('Z'-'A') + 1));

    return ttmsg;
}
int Application::getMaxNReplica(){
    return maxNReplica;
}
void Application::setMaxNReplica(int numberOfReplica){
    maxNReplica = numberOfReplica;
}
int Application::getClientID()
{
    return clientID;
}
void Application::setClientID(int clientID)
{
    clientID = clientID;
}

