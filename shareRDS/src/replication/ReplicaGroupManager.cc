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

#include "ReplicaGroupManager.h"

Define_Module(ReplicaGroupManager);

void ReplicaGroupManager::initialize()
{
    //I start  as an alive client
    dead = false;
    //Initialize the replica ID of the replica that has the group manager module
    replicaID = par("replicaID");
    //Validating that a replica ID was defined
    clientID = par("clientID");
    //Reincarnation counter set to 0
    reincCounter = 0;
    if(replicaID == -1)
        throw cRuntimeError("Invalid replica ID %d; must be >= 0", replicaID);

    //I receive the sequence of IDs in a string, here i divide them into tokens
    cStringTokenizer tokenizer (par("ReplicaIDs"));
    while(tokenizer.hasMoreTokens())
        ReplicaIDs.push_back(atoi(tokenizer.nextToken()));
}

int ReplicaGroupManager::getClientID(){
    return clientID;
}
void ReplicaGroupManager::setClientID(int clientID){
    this->clientID = clientID;
}
SystemMsg* ReplicaGroupManager::generateReincarnationMessage(int replica, int clientID){
    SystemMsg * ttmsg = new SystemMsg();
    ttmsg->setClientID(clientID);
    ttmsg->setReplicaID(replica);
    ttmsg->setReplyCode(NONE);
    ttmsg->setOperation(REINC);
    return ttmsg;
    }
void ReplicaGroupManager::handleMessage(cMessage *msg)
{
    //GATE in0 = WriteAheadProtocol
    //GATE in1 = ClientReincarnation
    //GATE in2 = InvocationManager
    //GATE in3 = ReplicaGroupManager itself

    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    int gateID = ttmsg->getArrivalGateId();

    if ( ttmsg->isSelfMessage()){
        //In this piece of code i send one reincarnation message and then i schedule the next one
        if(reincCounter < ReplicaIDs.size()){
            send(generateReincarnationMessage(ReplicaIDs.at(reincCounter), clientID),"out", TO_NETWORK);// i send messages to all my connected replica.
            std::cout << "sended message to" <<ReplicaIDs.at(reincCounter) << std::endl;
            reincCounter++;
            scheduleAt(simTime()+0.1, ttmsg);
        }
        else{
            delete ttmsg;
        }
    }
    else if (gateID == gate("in", FROM_WRITEAHEADPROTOCOL)->getId()){
        if (!dead){
            //here i forward the message directly to the
            //the basic network
            send(ttmsg,"out",TO_NETWORK);
        }
        else{
            delete ttmsg;
        }
    }
    else if (gateID == gate("in", FROM_CLIENTREINCARNATION)->getId()){
      // if(willfail){
        if(ttmsg->getReplyCode() == 1){
            // In this if branch I send a broadcast to all my connected replica
            // to say i'm alive
            // here i send the first reincarnation message and then I schedule the next if it exist
            reincCounter = 0;

            //for (i=0;i<ReplicaGroupManager::ReplicaIDs.length();i++){
            send(ReplicaGroupManager::generateReincarnationMessage(ReplicaIDs.at(reincCounter), clientID),"out",TO_NETWORK);// i send messages to all my connected replica.
            reincCounter++;
            SystemMsg* nextReincMSG = new SystemMsg();
            scheduleAt(simTime()+0.1, nextReincMSG);
            //}
            std::cout << "REPLICAGROUPMANAGER I'm alive "  <<std::endl;
            dead = false;
        }
        else{
            std::cout << "REPLICAGROUPMANAGER I'm dead "  <<std::endl;
            dead = true;
        }
       //}
        delete ttmsg;
    }
    else if (gateID == gate("in", FROM_INVOCATIONMANAGER)->getId()){
        //I put a valid ReplicaID
        //if I receive a message from the invocation manager
        //I'll forward directly to the WriteAheadProtocol
        //to log it
        if (!dead){
            int res_oracle = intuniform(0,ReplicaIDs.size()-1);
            ttmsg->setReplicaID(ReplicaIDs.at(res_oracle));
            send(ttmsg,"out", TO_WRITEAHEADPROTOCOL);
        }
        else{
            delete ttmsg;
        }
    }
}
