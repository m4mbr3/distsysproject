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

RemoteWriteProtocol::RemoteWriteProtocol()
{
    replicaID =-1;
    ownDataItem = false;
}

RemoteWriteProtocol::~RemoteWriteProtocol()
{
    dataItemsOwners.clear();
}

void RemoteWriteProtocol::initialize()
{
    //Initialize the replica ID of the replica that has the group manager module
    replicaID = par("replicaID");
    //Validating that a replica ID was defined
    if(replicaID == -1)
        throw cRuntimeError("REPLICA_WRITE_PROTOCOL: Invalid replica ID %d; must be >= 0", replicaID);
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
    //Retrieving the replica sender ID
     int msgReplicaID = sMsg->getReplicaID();
     //Retrieving the replica owner ID
     int msgReplicaOwnerID = sMsg->getReplicaOwnerID();
    //We receive a message from the write ahead protocol module
    if(gateID== gate("in",WAP_IN_GATE)->getId())
    {
        //We keep the operation WRITE
         sMsg->setOperation(WRITE);
         //If it is an answer for a rollaback
        if(msgOperationID == ROLLBACK && msgReplyCode == SUCCESS)
        {
            //We rollback the write done on the local data items
            send(msg, "out", DIM_OUT_GATE);
        }
        //if is a rollback of a just created data item, then we will need to delete it
        else if(msgOperationID == DELETE && msgReplyCode == SUCCESS)
        {
            //We keep the operation DELETE
             sMsg->setOperation(DELETE);
             //We delete the data item
             send(msg, "out", DIM_OUT_GATE);
        }
        //If the message through the network was successfully sent and executed on the other replicas
        else if(msgOperationID == COMMIT && msgReplyCode == SUCCESS)
        {
            //We answer correctly to the requester through the invocation manager
            send(msg, "out", IM_OUT_GATE);
        }
        //If it was a request for logging a write request
        else if(msgOperationID == UPDATE && msgReplyCode == SUCCESS)
        {
            //We reflect the write in the local data items
            send(msg, "out", DIM_OUT_GATE);
        }
        //Something really bad happen :(, we rise an exception
        else if(msgReplyCode == FAIL){
            //send(sMsg, "out", IM_OUT_GATE);
            throw cRuntimeError("REPLICA_WRITE_PROTOCOL:(1) A strange error happens when logging a write request in the replica %d", replicaID);
        }
    }
    //We read/ write in our local data items
    else if(gateID== gate("in",DIM_IN_GATE)->getId() ){
        //have executed succesfully a read/write request
        if(msgReplyCode == SUCCESS){
            //We write an owned data item
            if(msgReplicaOwnerID== replicaID && msgOperationID == WRITE){
                //We send the update to all the other replica by using multicasting
                send(sMsg, "out", RU_OUT_GATE);
                //We check if we need to answer a remote write request, the msg comes from another replica and i am the owner
                if(msgReplicaID != NO_REPLICA){
                    //We send the answer to the remote write request through invocation manager
                    send(sMsg->dup(), "out", IM_OUT_GATE);
                }

            }
            //We write locally and if the writing is from a data item that was written remotely on the owner replica and is requiring
            //an update/creation in the current replica
            else if(msgReplicaOwnerID!= replicaID && msgOperationID == WRITE)
            {
                //We answer that the request has finished to the requester
                send(sMsg, "out", IM_OUT_GATE);
            }
            //We have to send back the answer to the client/replica that sent the request a write/update
            //we send the answer by using the invocation manager, from which we have received this kind of messages
            else if (msgOperationID == READ){
                 send(sMsg, "out", IM_OUT_GATE);
            }
        }
        //if we couldnt read/write we rise and exception because the replica is assume to not fail
        else if(msgReplyCode == FAIL)
        {
            //send(sMsg, "out", IM_OUT_GATE);
            throw cRuntimeError("REPLICA_WRITE_PROTOCOL: (2) A strange error happens when logging a write request in the replica %d", replicaID);
        }


    }
    //We receive an answer from a replica to which we requested a remote write
     else if(gateID== gate("in",RW_IN_GATE)->getId()){
         //if the remote write or update succeed
         if(msgReplyCode == SUCCESS)
         {
             //We write locally
             send(msg, "out", DIM_OUT_GATE);
         }
         //The remote write failed because something bad happen during the communication
         else if (msgReplyCode== FAIL)
         {
             //We send the failure message to the client using the invocation manager
             send(sMsg, "out", IM_OUT_GATE);
         }
     }
    //We receive an update message answer from the whole replicas, this is validated by the
    //totally ordered when all multicasted messages  are ack!!
     else if (gateID == gate("in",RU_IN_GATE)->getId())
     {
         //The multicast worked fine, and therefore we confirm our local write
         if(msgReplyCode == SUCCESS){
             //We confirm the written done before multicasting
             sMsg->setOperation(COMMIT);
             send(msg,"out", WAP_OUT_GATE);
         }
         //Something got bad on when multicasting the update, then we need to roll back our local write
         else if (msgReplyCode== FAIL)
          {
             //need to do roll back of the local writing operation (creation or update!)
             sMsg->setOperation(ROLLBACK);
             //We retrieve the previous value from the log
             send(msg,"out", WAP_OUT_GATE);
          }

     }
    /**
     * 2. We received a message from the network:
     * -A client request: read/write
     * -A replica request: update data item value
     */
     else if(gateID == gate("in",IM_IN_GATE)->getId())
     {

         // The message is a write operation, so it can come from a client or from a replica
         if(msgOperationID == WRITE)
         {
             //We need to check if the data item is owned by one of the replicas, or if it is the
             //first time that it is created (locally) or in a remote replica that is sending us an update
            int replica = -1;
            try{
                //checking if the data item with the given ID already exists in the system
                replica =dataItemsOwners.at(msgDataID);
                //If it exist, and the message do not come from a replica (comes from a client), and the
                //the owner of the data item is the current replica
                if(msgReplicaID== NO_REPLICA && replica == replicaID)
                {
                   //We update the owner field of the message
                   sMsg->setReplicaOwnerID(replicaID);
                   // we log the write in the case the update to other replicas cannot be done,and therefore we should able to revert the write
                   sMsg->setOperation(UPDATE);
                   send(msg,"out", WAP_OUT_GATE);
                }
                //If it exists, and the message comes from another replica, then we check that
                //actually the data items belong to the replica id in the message and then meaning that this is
                //an update
                else if(msgReplicaID!= NO_REPLICA && replica == msgReplicaOwnerID && replica!= replicaID)
                {
                    //We should execute such write MANDATORY, and because the replica does not fail the local write MUST not fail
                    send(msg, "out", DIM_OUT_GATE);
                }
                //If it exists, and the message comes from another replica, then we check that
               //actually the data items belong to this replica id  and then meaning that this is
               //a remote write
                else if(msgReplicaID!= NO_REPLICA && replica == msgReplicaOwnerID && replica== replicaID){
                    //We should execute such write MANDATORY, and because the replica does not fail the local write MUST not fail
                    send(msg, "out", DIM_OUT_GATE);
                }
                //if it exists, and the message do not have a replica ID, then the request comes from a client
                //and the current replica is not the owner (is another replica) then we send a remote write
                else if(msgReplicaID== NO_REPLICA && replica!= replicaID)
                {
                   //We update the owner field of the message
                   sMsg->setReplicaOwnerID(replica);
                   // We send the update of the data item to the remote replica that owns it
                   send(msg, "out", RW_OUT_GATE);
                }

             }
            catch (const std::out_of_range& e)
            {
                //the data items does not exists in the system
                //the data item is send by a client and therefore the data item belongs to the current replica
               if(msgReplicaID== NO_REPLICA){
                   //We relate the replica owner ID with the data item id
                    dataItemsOwners[msgDataID] =replicaID;
                   //We update the owner field of the message
                   sMsg->setReplicaOwnerID(replicaID);
               }
                //if it is coming from an update from other replica that has written for the first time the data item
               //in its data items manager
               //is an update
               else if(msgReplicaID!= NO_REPLICA)
                 {
                     //We relate the replica owner ID with the data item id
                     dataItemsOwners[msgDataID] =msgReplicaOwnerID;
                 }

               //log the write because the message will be send through the network, which can fail and therefore we must rollback the
               //the related executed writings
               sMsg->setOperation(UPDATE);
               send(msg,"out", WAP_OUT_GATE);

               //send(msg, "out", DIM_OUT_GATE);
            }
         }
         //if the request is a read, we read the current value locally the data items manager should check the
         //time stamp of the read.
          else if(msgOperationID == READ)
          {
              send(msg, "out", DIM_OUT_GATE);
          }
     }

}
