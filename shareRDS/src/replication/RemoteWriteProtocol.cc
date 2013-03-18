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
     //We initialized our flag
    newDataItem = false;
}

void RemoteWriteProtocol::handleMessage(cMessage *msg)
{
    //We retrieve the msg
     SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
    //Retrieving the id of the gate from which we have received the message
    int gateID= sMsg->getArrivalGateId();
    //retrieving the data ID from the message
     std::string msgDataID = sMsg->getDataID();
    /**
     * 1.Checking if it is answer of a previous message sent
     */
    //1.1 Checking which kind of operation is involved on the message
    int msgOperationID = sMsg->getOperation();
    //1.2 We retrieved the reply code
    int msgReplyCode = sMsg->getReplyCode();
    //1.3 Checking the success of the message
    //We read/ write successfully in our owned data items
    if(gateID== DIM_IN_GATE  && msgReplyCode == SUCCESS){
        //If it was a write then we should notify to the other replicas
        if(msgOperationID == WRITE)
        {
            //We write an owned data item
            if(newDataItem){
                //We set up the replica ID that will generate a request for a write or a read of a data item in the system
                 sMsg->setReplicaID(replicaID);
                //We send the update to the multicast component such that we propagate the update
                send(sMsg, "out", RU_OUT_GATE);
            }
            //We have to send back the answer to the replica that owns the data item
            else{
                send(sMsg, "out", RW_OUT_GATE);
            }

        }
        if(msgOperationID == READ)
        {
            //We answer to the client the read done, the msg is send to the basic network module
            send(sMsg, "out", CL_OUT_GATE);
        }
        //TODO Release the lock on the data item

    }
    //We acquire the lock of a data item
    else if(gateID== ME_IN_GATE  && msgReplyCode == SUCCESS){
         //We write or read the data
         //TODO do we need to acquire lock for reading??
         //We read/write locally
         if(msgOperationID == READ || newDataItem)
         {
             //We read the data
             send(sMsg, "out", DIM_OUT_GATE);
         }
         //Remote write
         else if(!newDataItem){
             //We set up the replica ID that will generate a request for a write or a read of a data item in the system
             sMsg->setReplicaID(replicaID);
             send(sMsg, "out", RW_OUT_GATE);
         }
     }
     //we couldnt obtain the lock, something bad happened :(
     else if (gateID== ME_IN_GATE  && msgReplyCode == FAIL)
     {
         throw cRuntimeError("The lock was not possible to acquire on replica ID %d for data ID %s", replicaID,sMsg->getDataID());
     }
     //We receive an answer from a replica to which we requested a remote write or a remote update
     else if(gateID== RW_IN_GATE || gateID== RU_IN_GATE){
         //We send the answer to the client
         send(sMsg, "out", CL_OUT_GATE);
     }
    //We receive an update message from other replica
     else if (gateID == RU_IN_GATE)
     {
         //We save the owner of the data ID received
         int msgReplicaID = sMsg->getReplicaID();
         dataItemsOwners[msgDataID] =msgReplicaID;
         //We reflect the change locally
         send(sMsg, "out", DIM_OUT_GATE);
     }
     /**
      * 2. We send a message to the mutual exclusion component for acquiring the lock
      */
     else{

        //If it is a write, we need to check if the current replica owned the data item with the ID received
        if(msgOperationID == WRITE){
           int replica = -1;
           try{
               //checking if the data item with the given ID already exists in the system
               replica =dataItemsOwners.at(msgDataID);
            }
           catch (const std::out_of_range& e)
           {
               //Then it is a new data item in the whole system
               newDataItem = true;
           }

        }

        if(!newDataItem)
            send(msg, "out", ME_OUT_GATE);
        else
            send(msg, "out", ME_OUT_GATE);

     }

}
