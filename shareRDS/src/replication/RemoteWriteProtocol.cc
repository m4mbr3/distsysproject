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

#include "RemoteWriteProtocol.h"
#include "SystemMsg_m.h"

Define_Module(RemoteWriteProtocol);

void RemoteWriteProtocol::initialize()
{
    //Initialize the replica ID of the replica that has the group manager module
    replicaID = par("replicaID");
    //Validating that a replica ID was defined
    if(replicaID == -1)
        throw cRuntimeError("Invalid replica ID %d; must be >= 0", replicaID);
}

void RemoteWriteProtocol::handleMessage(cMessage *msg)
{
    //1. We retrieve the msg
     SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
    //2.The name of the message received!!, IT SHOULD HAVE A NAME SET UP BY THE SENDER
    EV<< "REPLICA_WRITE_PROTOCOL ("<< replicaID <<") Received message from the Invocation manager: " << sMsg->getName();
    //3. Retrieving the replica ID from the received the message
    int msgReplicaID = sMsg->getReplicaID();
    //4. retrieving the data ID from the message
    std::string msgDataID = sMsg->getDataID();
    //5. Checking if its a message from a Replica in the system or from a client
    //If it comes from the client
    if(msgReplicaID == NO_REPLICA)
    {

        try{
            //checking if the data item with the given ID already exists on the current replica
            ownedDataItems.at(msgDataID);
         }
        catch (const std::out_of_range& e)
        {
            //It is the first time writting the data item
            ownedDataItems[msgDataID] = true;
        }

    }
    //it comes from a replica and then it is an update to data item owned by the sending replica
    else
    {
        ownedDataItems[msgDataID] = false;
    }

    //Resending it to the distributed mutual exclusion component
    EV<< "REPLICA_WRITE_PROTOCOL ("<< replicaID <<") Sending message to the DS mutual exclusion module: " << sMsg->getName();
    send(msg, "out");

}
