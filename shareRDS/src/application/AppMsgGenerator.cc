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
    //Assigning a replica
    replicaID = intuniform(0, nReplicas-1);
    //we start by sending writes requests
    WATCH(dataID);
    //building the first message to send
    timeToSendMessage = new cMessage("clientReqTimer");
    //Scheduling another sending of the message
    scheduleAt(simTime() + exponential(1.0), timeToSendMessage);
    //initializing our local clock
    localClock = 0;
    //Initializing the counter
    counter = 0;

}

cMessage* AppMsgGenerator::getMessage(){

    SystemMsg* sMsg = new SystemMsg("msgFromAppMsgGenerator");
//    SystemMsg* sMsg = new SystemMsg("ReqMsg");
//    SystemMsg* sMsg = new SystemMsg("ClientReqMsg");
    //Client D
    sMsg->setClientID(cID);
    //Data ID
    //TODO change it according to the test that you need to do
    dataID = "a";
    sMsg->setDataID(dataID.c_str());
    //data
    sMsg->setData(intuniform(-1000, 1000));
    //if it is the first message it should be a write
    //TODO we can also simulate a read for ERROR HANDLING
    if(counter ==0)
    {
        sMsg->setOperation(WRITE);
    }
    else{
        int op = intuniform(0, 1);
        if(op== WRITE)
            sMsg->setOperation(WRITE);
        else
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
        send(m, "replicasOut", replicaID);
        //Scheduling another sending of the message
        scheduleAt(simTime() + exponential(1.0), timeToSendMessage);
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
