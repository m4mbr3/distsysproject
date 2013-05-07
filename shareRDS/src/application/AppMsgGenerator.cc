//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "AppMsgGenerator.h"


Define_Module(AppMsgGenerator);

AppMsgGenerator::AppMsgGenerator(){
    timeToSendMessage=  NULL;
    cID =-1;
    nReplicas =-1;
}

AppMsgGenerator::~AppMsgGenerator()
{
    cancelAndDelete(timeToSendMessage);
}

void AppMsgGenerator::initialize()
{
    /**
     * RETRIEVING THE SIMULATION PARAMETERS
     */
    //We recover the component id
    cID = par("clientID");
    //Validating that a replica ID was defined
    if(cID == -1)
        throw cRuntimeError("Invalid client ID %d; must be >= 0", cID);
    //Number of replicas
    nReplicas = par("numberReplicas");
    //Validating that a replica ID was defined
    if(nReplicas == -1)
     throw cRuntimeError("Invalid number of replicas %d; must be >= 0", nReplicas);
    //We recover the operations order
    const char * ops = par("operationOrder").stringValue();
    operations = cStringTokenizer(ops).asVector();
    //We recover the data items ids
    const char* di= par("dataItemsID").stringValue();
    dataItemsId = cStringTokenizer(di).asVector();
    //Validation for checking that the two vectors have the same size
    int s1 = operations.size();
    int s2 = dataItemsId.size();
    if(s1 != s2)
        throw cRuntimeError("The number of operations must be equal to the number of data items in client %d", cID);
    //Initializing the counter
    counter = 0;
    //Assigning the replica id defining in the ned parameter
    replicaID = par("replicaId");
    /**
     * CONFIGURING THE SELF MESSAGE FOR KEEP SENDING MESSAGES TO THE REPLICA
     */
    //Retrieving the time for the send message timer
    timerOffset = par("timerOffset").doubleValue();
    //building the first message to send
    timeToSendMessage = new cMessage("clientReqTimer");
    //Scheduling another sending of the message
    scheduleAt(simTime() + exponential(timerOffset), timeToSendMessage);
    /**
     * CONFIGURING THE LOCAL CLOCK AND WATCHERS
     */
    //initializing our local clock
    localClock = 0;
    //we watch which is the data item involved in the message
    WATCH(dataID);


}

cMessage* AppMsgGenerator::getMessage(){

    //Validation of the number of msgs already sent
    int max = operations.size();
    //if we already sent max number of msgs
    if(counter == max)
        return NULL;
    //otherwise create a message
    SystemMsg* sMsg = new SystemMsg("ClientReqMsg");
    //Client D
    sMsg->setClientID(cID);
    //Data ID
    dataID = dataItemsId[counter];
    sMsg->setDataID(dataID.c_str());
    //Data generated randomly
    sMsg->setData(intuniform(-1000, 1000));
    //We retrieve the operation involved int the message
    std::string op = operations[counter];
    if(op.compare("w") ==0)
    {
        sMsg->setOperation(WRITE);
    }
    else if (op.compare("r") ==0){
         sMsg->setOperation(READ);
    }
    //timestamp
    localClock++;
    sMsg->setLamportClock(localClock);
    //We increment the counter
    counter++;
    return sMsg;

}

void AppMsgGenerator::handleMessage(cMessage *msg)
{
    if(msg == timeToSendMessage)
    {
        //Building the message
        cMessage * m = getMessage();
        //Sending the message
        if(m!= NULL)
        {
            send(m, "replicasOut", replicaID);
            //Scheduling another sending of the message
            scheduleAt(simTime() + exponential(1.0), timeToSendMessage);
        }
    }
    else{

        int gateID = msg->getArrivalGateId();
        //We receive a message of success of a writing or a read
        if(gateID == findGate("in")){
            //we delete the message because this is the end of it life cycle
            delete msg;
        }
    }

}
