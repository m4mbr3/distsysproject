#include "Application.h"

Define_Module(Application);
void Application::initialize()
{
    setClientID(par("clientID"));
    if (clientID == -1 )
        throw cRuntimeError ("Invalid client ID %d;must be >= 0", clientID );
    ttmsg = new SystemMsg();
    scheduleAt(3.0, ttmsg);
}

void Application::handleMessage(cMessage *msg)
{
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    if (ttmsg->isSelfMessage()){
        // I received a message from myself
        scheduleAt(intuniform(4,20), ttmsg);
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
SystemMsg* Application::generateMessage(){
    //This is the function that generates random messages to replica

    SystemMsg *ttmsg = new SystemMsg();
    ttmsg->setClientID(clientID);
    ttmsg->setOperation(intuniform(0,1));
    ttmsg->setData(intuniform(-1000, 1000));
    ttmsg->setDataID(((intuniform(0,100)%2)==0) ?(const char *) 'a' + rand() % (('z'-'a') + 1): (const char *)'A' + rand() % (('Z'-'A') + 1));

    return ttmsg;
}
int Application::getClientID()
{
    return clientID;
}
void Application::setClientID(int clientID)
{
    clientID = clientID;
}

