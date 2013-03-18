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
    ownDataItem = false;

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
    if(gateID== DIM_IN_GATE) {
        //have executed succesfully a read/write request
        if(msgReplyCode == SUCCESS){
            //We write an owned data item
            if(ownDataItem && msgOperationID == WRITE){
                //We set up the replica ID that will generate a request for a write or a read of a data item in the system
                 sMsg->setReplicaID(replicaID);
                //We send the update to the multicast component such that we propagate the update
                send(sMsg, "out", RU_OUT_GATE);
            }
            //We have to send back the answer to the client/replica that sent the request a write/update
            //we send the answer by using the invocation manager, from which we have received this kind of messages
            else if (msgOperationID == READ){
                 send(sMsg, "out", IM_OUT_GATE);
            }
        }
        //if we couldnt read/write we answer with a fail reply code
        else if(msgReplyCode == FAIL)
        {
            send(sMsg, "out", IM_OUT_GATE);
        }


    }
    //We receive an answer from a replica to which we requested a remote write
     else if(gateID== RW_IN_GATE){
         //if the remote write or update succeed
         if(msgReplyCode == SUCCESS)
         {
             //We now write locally on the local data items
             send(sMsg, "out", DIM_IN_GATE);
         }
         else if (msgReplyCode== FAIL)
         {
             //We send the failure message to the client using the invocation manager
             send(sMsg, "out", IM_OUT_GATE);
         }


     }
    //We receive an update message answer from the whole replicas, this is validated by the
    //totally ordered when all multicasted messages  are ack!!
     else if (gateID == RU_IN_GATE)
     {
         if(msgReplyCode == SUCCESS){
             //we send back the answer to the client w/o changing any locally
             send(sMsg, "out", IM_OUT_GATE);
         }
         else if (msgReplyCode== FAIL)
          {
             //TODO need to do roll back of the local writing operation
          }

     }
    /**
     * 2. We received a message from the network:
     * -A client request: read/write
     * -A replica request: update data item value
     */
     else if(gateID == IM_IN_GATE)
     {
         // If the received message comes from another replica it means that it is an update
         // To a data item that the remote replica owns.
         int msgReplicaID = sMsg->getReplicaID();
         // The message is a write operation, so it can come from a client or from a replica
         if(msgOperationID == WRITE)
         {
             //We need to check if the data item is owned by one of the replicas, or if it is the
             //first time that it is created
            int replica = -1;
            try{
                //checking if the data item with the given ID already exists in the system
                replica =dataItemsOwners.at(msgDataID);
                //If it exist and the message do not come from a replica, and the
                //the owner of the data item is the current replica
                if(msgReplicaID== NO_REPLICA && replica == replicaID)
                {
                    //we update our flag
                    ownDataItem = true;
                   // TODO we log the write in the case the update is cancel for some reason
                   // We send the update of the data item to the local data items manager
                   send(msg, "out", DIM_OUT_GATE);
                }
                //If it exists and the message comes from another replica, then we check that
                //actually the data items belong to the replica id in the message and so it means this is
                //an update
                else if(msgReplicaID!= NO_REPLICA && replica == msgReplicaID)
                {
                    //we update the owned flag
                    ownDataItem = false;
                    // TODO we log the write in the case the update is cancel for some reason
                    // We send the update of the data item to the local data items manager
                    send(msg, "out", DIM_OUT_GATE);
                }
                //if it exists and the message do not have a replica ID, then the request comes from a client
                //and the owner is another replica (not the current) then we send a remote write
                else if(msgReplicaID== NO_REPLICA && replica!= NO_REPLICA)
                {
                    //we update our flag
                   ownDataItem = false;
                  // TODO we log the write in the case the update is cancel for some reason
                  // We send the update of the data item to the remote replica that owns it
                  send(msg, "out", RW_OUT_GATE);
                }
             }
            catch (const std::out_of_range& e)
            {
                //the data items does not exists in the system
                //the data item is send by a client and therefore the data item belongs to the current replica
               if(msgReplicaID== NO_REPLICA){
                   //Then it is a new data item in the whole system and the owner is the current replica
                   ownDataItem = true;
               }
                //if it is coming from an update from other replica that has written for the first time the data item
               //in its data items manager
               else if(msgReplicaID!= NO_REPLICA)
                 {
                     //We relate the replica ID with the data item id
                     dataItemsOwners[msgDataID] =msgReplicaID;
                     ownDataItem = false;
                 }
               //TODO log the write in the case the creation of the data item is cancel for some reason
               //We write the data item involved in the message
               send(msg, "out", DIM_OUT_GATE);

            }
         }
     }
    //if the request is a read, we read the current value locally the data items manager should check the
    //time stamp of the read.
     else if(msgOperationID == READ)
     {
         send(msg, "out", DIM_OUT_GATE);
     }
}
