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
    timeToSendMessage = new cMessage("sendMsg");
    //Scheduling another sending of the message
    scheduleAt(simTime() + exponential(1.0), timeToSendMessage);
    //initializing our local clock
    localClock = 0;

}

cMessage* AppMsgGenerator::getMessage(){

<<<<<<< HEAD
    SystemMsg* sMsg = new SystemMsg("msgFromAppMsgGenerator");
=======
    SystemMsg* sMsg = new SystemMsg("ReqMsg");
>>>>>>> b29bdea3f846a3daed214966b018c8ab1d84e8d2
    //Client D
    sMsg->setClientID(cID);
    //Data ID
    dataID = "a";
    sMsg->setDataID(dataID.c_str());
    //data
    sMsg->setData(intuniform(-1000, 1000));
    //operation
    int op = intuniform(0, 1);
    if(op== WRITE)
        sMsg->setOperation(WRITE);
    else
        sMsg->setOperation(READ);
    //timestamp
    localClock++;
    sMsg->setLamportClock(localClock);
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
    /*
    else{
        int gateID = msg->getArrivalGateId();
        //We receive a message of success of a writing or a read
        if(gateID = gate("inReplicas", replicaID)){

        }
    }
    */
}
