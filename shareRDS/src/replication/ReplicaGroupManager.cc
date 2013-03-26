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
#include "SystemMsg_m.h"

Define_Module(ReplicaGroupManager);

void ReplicaGroupManager::initialize()
{
    //I start  as an alive client
    dead = false;
    //Initialize the replica ID of the replica that has the group manager module
    replicaID = par("replicaID");
    //Validating that a replica ID was defined
    clientID = par("clientID");
    if(replicaID == -1)
        throw cRuntimeError("Invalid replica ID %d; must be >= 0", replicaID);

    //I receive the sequence of IDs in a string, here i divide them into tokens
    cStringTokenizer tokenizer (par("ReplicaIDs"));
    while(tokenizer.hasMoreTokens())
        ReplicaIDs.push_back(tokenizer.nextToken());
}

int ReplicaGroupManager::getClientID(){
    return clientID;
}
void ReplicaGroupManager::setClientID(int clientID){
    this.clientID = clientID;
}
SystemMsg* ReplicaGroupManager::generateReincarnationMessage(int replica, int clientID){
    SystemMsg * ttmsg = new SystemMsg();
    ttmsg->setClientID(clientID);
    ttmsg->setReplicaID(replica);
    ttmsg->setIsClientReincarnation(true);
    return ttmsg;
    }
void ReplicaGroupManager::handleMessage(cMessage *msg)
{
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);

    if (ttmsg->getIsClientReincarnation()){
        if(ttmsg->getReplyCode() == 1){
            // In this if branch I send a broadcast to all my connected replica
            // to say i'm alive
            int i;
            for (i=0;i<ReplicaGroupManager::ReplicaIDs.length();i++){
                send(ReplicaGroupManager::generateReincarnationMessage(ReplicaIDs.at(i), this.clientID),"out2");// i send messages to all my connected replica.
            }
            EV << "REPLICAGROUPMANAGER sended alive messages to every replica " << endl;
            dead =false;
        }
        else{
            EV << "REPLICAGROUPMANAGER I'm dead "  <<endl;
            dead = true;
        }
    }
    if(!dead){
        if (){
            //TODO other stuff

        }
    }
    delete ttmsg;
}
