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
    //Initialize the module with the number of replicas provided
    nReplicas = par("numberReplicas");
    //Initialize the replica ID of the replica that has the group manager module
    replicaID = par("replicaID");
    //Validating that a replica ID was defined
   :
    if(replicaID == -1)
        throw cRuntimeError("Invalid replica ID %d; must be >= 0", replicaID);

}


void ReplicaGroupManager::handleMessage(cMessage *msg)
{
    //1. We retrieve the msg
    SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
    //2.The name of the message received!!, IT SHOULD HAVE A NAME SET UP BY THE SENDER
    EV<< "REPLICA_GROUP_MANAGER ("<< replicaID <<") Received message: " << sMsg->getName();
    //3.Sending the message to the module in charge of managing the replication protocol
    send(msg, "out");
    EV<< "REPLICA_GROUP_MANAGER ("<< replicaID <<") Received message: " << sMsg->getName();

}
