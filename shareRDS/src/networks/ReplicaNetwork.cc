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


#include "ReplicaNetwork.h"
#include "SystemMsg_m.h"
#include <vector>

Define_Module(ReplicaNetwork);

ReplicaNetwork::ReplicaNetwork()
{
    myReplicaID = -1;
    timeToSendOutRequest = NULL;
    timeToSendOutRequest = NULL;
    timeToCheckAcks = NULL;
    lamportClock = 0;
    lcLastMsgSent = lamportClock;
}

ReplicaNetwork::~ReplicaNetwork()
{
    //Cleaning up the maps
    msgsAck.clear();
    //This should have all the references should already be deleted
    /*
    std::map<long, SystemMsg*>::iterator it;
    for (it = msgsWaitingForAck.begin(); it != msgsWaitingForAck.end(); it++)
    {
        SystemMsg* m = it->second;
        delete m;
    }
    */
    msgsWaitingForAck.clear();
    //Cleaning up the vectors, the memory space pointed by the pointers should have been already emptied
    //Cleaning the in queue
    std::vector<SystemMsg*>::iterator it2;
    for (it2 = inQueue.begin(); it2 != inQueue.end(); it2++)
    {
        SystemMsg* m = *it2;
        delete m;
    }
    inQueue.clear();
    //Cleaning the out queue
    for (it2 = outQueue.begin(); it2 != outQueue.end(); it2++)
    {
        SystemMsg* m = *it2;
        delete m;
    }
    outQueue.clear();
    //Cancelling and deleting the timers
    cancelAndDelete(timeToSendOutRequest);
    cancelAndDelete(timeToProcessRequest);
    cancelAndDelete(timeToCheckAcks);
}

void ReplicaNetwork::initialize()
{
    // Initialize the replica ID of the replica that has the group manager module
    myReplicaID = par("replicaID");
    // Validating that a replica ID was defined
    if(myReplicaID == -1)
        throw cRuntimeError("REPLICA NETWORK: Invalid replica ID %d; must be >= 0", myReplicaID);

    lamportClock = 0;
    WATCH(lamportClock);

    lcLastMsgSent = -1;
    WATCH(lcLastMsgSent);

    timeToProcessRequest = new cMessage("processingRequestTimer");

    // schedule to send msg to request to process queuing msg
    // send this to InvocationManager
    pTimerOffset = par("processingTimerOffset").doubleValue();
    scheduleAt(simTime() + exponential(pTimerOffset), timeToProcessRequest);

    sTimerOffset = par("sendingTimerOffset").doubleValue();
    timeToSendOutRequest = new cMessage("sendRequestTimer");
    // schedule to process outQueue
    scheduleAt(simTime() + exponential(sTimerOffset), timeToSendOutRequest);

    caTimerOffset =  par("checkAckTimerOffset").doubleValue();
    timeToCheckAcks = new cMessage("checkAcksTimer");
}

void ReplicaNetwork::lamportClockHandle(SystemMsg *msg) {
    int rcvLamportClock =msg->getLamportClock();
    // updating the local clock of the module usign the received value
    if(rcvLamportClock >lamportClock)
    {
        lamportClock = rcvLamportClock +1;
    }
    else
    {
        lamportClock = lamportClock + 1;
    }
}

void ReplicaNetwork::handleMessage(cMessage *msg)
{

    // Time to start processing the messages saved in the inQueue
    if (msg == timeToProcessRequest) {
        int size = inQueue.size();
        for (int i = 0; i < size; i++) {
            //We retrieve the message
            SystemMsg* sMsg = inQueue.at(i);
            // retrieve the gateID of the gate from which we received the msg
            int gateID = sMsg->getArrivalGateId();
            // retrieve other data from the msg
            int msgClientID = sMsg->getClientID();
            int msgReplicaID = sMsg->getReplicaID();
            int msgReplyCode = sMsg->getReplyCode();
            int msgOperation = sMsg->getOperation();
            int msgOwnerReplicaID = sMsg->getReplicaOwnerID();
            std::string msgDataID = sMsg->getDataID();
            // retrieve the treeID of the msg because it won't change when cloning the msg
            int msgTreeID = sMsg->getTreeId();

            // the incoming msg from another replica and it is an ACK answer of a RemoteUpdate from the current replica
            if ((gateID == findGate("inReplicas", msgReplicaID)) && (msgOperation == ACK)) {
                std::vector<bool> inAck;   // a vector to store all ACKs received for this msg
                try {
                    inAck = msgsAck.at(msgTreeID);
                    inAck[msgReplicaID] = true;
                    msgsAck[msgTreeID]=inAck;
                    delete sMsg;


                } catch (const std::out_of_range& e) {
                    throw cRuntimeError("REPLICA NETWORK: An error occurred on processing acks in Replica with id %d", myReplicaID);
                }
            }
            // incoming msg is an answer for a RemoteWrite request that the current replica has request in a remote write request
            else if (msgReplicaID!= NO_REPLICA && (gateID == findGate("inReplicas", msgReplicaID)) && msgReplicaID == msgOwnerReplicaID && msgReplyCode == SUCCESS) {
                // check if the msg is on top of the outQueue or not
                // if it's on top then send
                send(sMsg->dup(), "outAnswer", RW_OUT_GATE);
                //We update the timestamp of the last message processed
                lcLastMsgSent = sMsg->getLamportClock();
                ///We delete the reference to the message
                //inQueue.erase(inQueue.begin()+i);
                //We delete the message
                delete sMsg;

            }
            //The incoming msg is a remote update request from another replica
            else if (gateID == findGate("inReplicas", msgReplicaID) && msgReplicaID == msgOwnerReplicaID && msgReplyCode == NO_REPLY_CODE){
                send(sMsg->dup(), "outRequest");
                //We update the timestamp of the last message processed
                lcLastMsgSent = sMsg->getLamportClock();
                //We delete the message
                delete sMsg;
            }
            //The incoming message is a remote write request
           else if (gateID == findGate("inReplicas", msgReplicaID) && msgReplicaID != msgOwnerReplicaID && myReplicaID == msgOwnerReplicaID ){
               send(sMsg->dup(), "outRequest");
               //We update the timestamp of the last message processed
               lcLastMsgSent = sMsg->getLamportClock();
               //We delete the message
               delete sMsg;
           }

            // incoming msg is from a Client
            else if (gateID == findGate("inClients", msgClientID)){
                send(sMsg->dup(), "outRequest");
                //We update the timestamp of the last message processed
                lcLastMsgSent = sMsg->getLamportClock();
                ///We delete the reference to the message
                //inQueue.erase(inQueue.begin()+i);
                //We delete the message
                delete sMsg;
            }
         }
        //We clear the in queue
        inQueue.clear();
        //we schedule again the processing timer
        scheduleAt(simTime() + exponential(pTimerOffset),timeToProcessRequest);
    }
    //Time to start processing the requests that are in the outqueue
    else if (msg == timeToSendOutRequest) {
        int size = outQueue.size();
        for (int i = 0; i < size; i++) {
            SystemMsg* sMsg = outQueue.at(i);
            // retrieve other data from the msg
            int msgClientID = sMsg->getClientID();
            int msgReplicaID = sMsg->getReplicaID();
            int msgReplicaOwnerID = sMsg->getReplicaOwnerID();
            int msgReplyCode = sMsg->getReplyCode();
            int gateID= sMsg->getArrivalGateId();
            std::string msgDataID = sMsg->getDataID();
            // retrieve the treeID of the msg because it won't change when cloning the msg
            int msgTreeID = sMsg->getTreeId();
            //We set up the sender of the message
            sMsg->setReplicaID(myReplicaID);
            //The msg is a request for a remote update from the current replica to the rest of the replicas in the system
            if (msgReplicaOwnerID == myReplicaID && gateID == findGate("inRemoteRequests", RU_IN_GATE)) {
                // multicast
                //If everything has been executed succesfully inside the current replica
                if (msgReplyCode == SUCCESS) {
                    //We same the message as a message to which we will need to check the acks from the other replicas
                    msgsWaitingForAck[msgTreeID] = sMsg->dup();
                    //We set up the sender of the message
                    msgsWaitingForAck[msgTreeID]->setReplicaID(msgReplicaID);
                    //We initialize the state of the acks of the involved message
                    int noOfReplicas = gateSize("outReplicas");
                    std::vector<bool> temp(noOfReplicas);
                    for (int i = 0; i < noOfReplicas; i++) {
                        temp[i] = false;
                    }
                    //We save the state of the acks of the involved message
                    msgsAck[msgTreeID] = temp;
                    /**
                     * Multicast
                     */
                    bubble("Multicasting");
                    //We multicast the messages, including ourselves
                    for (int i = 0; i< noOfReplicas; i++) {
                        //We send a duplicate of the current message
                        SystemMsg* outgoingMsg = sMsg->dup();
                        //We set up the reply code as -1, because we are waiting for an answer from a remote replica
                        outgoingMsg->setReplyCode(NO_REPLY_CODE);
                        //Changing the color of the multicast msgs
                        outgoingMsg->setKind(4);
                        //Sending out the msg
                        send(outgoingMsg, "outReplicas",i);
                    }
                    //Checking if the timer is already schedule
                    if(timeToCheckAcks->isScheduled())
                       {
                           cancelEvent(timeToCheckAcks);
                       }
                    //We schedule the timer for checking the state of the acks
                    if (timeToCheckAcks->isScheduled()) {
                        cancelEvent(timeToCheckAcks);
                    }
                    else {
                        scheduleAt(simTime() + exponential(caTimerOffset), timeToCheckAcks);
                    }
                }
            }
            //We need to send a request for a write request to the owner of the data item (a remote replica)
           else if(msgReplicaOwnerID != myReplicaID && gateID == findGate("inRemoteRequests",RW_IN_GATE))
           {
               //We send the msg to the owner of the data item
                send(sMsg->dup(), "outReplicas", msgReplicaOwnerID);
           }
            //incoming msg is an answer of a RemoteWrite request from a remote replica, we need to send it to the sender
            else if (msgReplicaID!=NO_REPLICA && msgReplicaID != myReplicaID && msgReplicaOwnerID == myReplicaID && gateID == findGate("inAnswer")){
                //We send the answer back to the sender
                send(sMsg->dup(), "outReplicas", msgReplicaID);
            }
            //The msg is an answer of a RemoteUpdate request from a remote replica and then we need to send it to the replicaOwnerID
            //In fact the replicaOwner on the message SHOULD be the same as the replica sender
            else if (msgReplicaID!=NO_REPLICA && msgReplicaID != myReplicaID && msgReplicaOwnerID != myReplicaID  && gateID == findGate("inAnswer")) {
                //We set up the operation as an ACK
                sMsg->setOperation(ACK);
                //We send a duplicate of the current message
                SystemMsg* outgoingMsg = sMsg->dup();
                //Changing the color of the multicast msgs
                //outgoingMsg->setKind(4);
                //We send the msg to the owner of the data item that has requested the update of the variable
                send(outgoingMsg, "outReplicas", msgReplicaOwnerID);
            }
            //The msg is an answer of a RemoteUpdate request from the current replica and then we need to send it to the replicaOwnerID
            //In fact the replicaOwner on the message SHOULD be the same as the replica sender
            else if (msgReplicaID!=NO_REPLICA && msgReplicaID == myReplicaID && msgReplicaOwnerID == myReplicaID  && gateID == findGate("inAnswer") && msgReplyCode ==SUCCESS) {
                //We set up the operation as an ACK
                sMsg->setOperation(ACK);
                //We send a duplicate of the current message
                SystemMsg* outgoingMsg = sMsg->dup();
                //Changing the color of the multicast msgs
                //outgoingMsg->setKind(4);
                //We send the msg to the owner of the data item that has requested the update of the variable
                send(outgoingMsg, "outReplicas", msgReplicaOwnerID);
            }
            //We are processing a message that is an answer to a client request
           else {
               send(sMsg->dup(), "outClients", msgClientID);
           }
            //We delete the message
            delete sMsg;
        }
        //We erase the outqueue
        outQueue.clear();
        //Rescheduling the time to process the outqueue
         scheduleAt(simTime() + exponential(sTimerOffset), timeToSendOutRequest);
    }
    //Checking the acks of a message
    else if (msg == timeToCheckAcks) {
        std::map<long, bool> msgToDelete;
        std::map<long, std::vector<bool> >::iterator it;
        for (it = msgsAck.begin(); it != msgsAck.end(); it++) {
            //The msg id of the msg for which we are waiting acks
            int msgID = it->first;
            //By default we dont need to delete this message becasue we do not know if it already has received all the acks
            msgToDelete[msgID] = false;
            // Retrieving the state of the acks of the current msg
            std::vector<bool> acks = it->second;
            //For validating if we already have ALL acks from all the replicas in the system
            bool okToSend = true;
            //The pointer to the message
            SystemMsg* m = msgsWaitingForAck[msgID];
            int size = acks.size();
            //We check if we have received all the acks of the current message (msgID)
            for (int i = 0; i < size; i++) {
                //Retrieving the state of the ack on the replica i
                bool acki = acks[i];
                // check if we already received the ACK or not, if not then we resend the request
                //TODO how many times do we try?
                /**
                 * Retrying the acks not received
                 */
                if (!acki) { // false
                    okToSend = false;
                    //We update the lamport clock because an event is happening inside the replica system
                   lamportClockHandle(m);
                   //Outgoing msg
                   SystemMsg* outgoingm= m->dup();
                   //Set up the new lamport clock of the message
                   outgoingm->setLamportClock(lamportClock);
                   //Set up the reply code as -1
                   outgoingm->setReplyCode(NO_REPLY_CODE);
                   //Put as another color the retry of a multicast
                   outgoingm->setKind(5);
                   //We put the current replica as the sender of the message
                   outgoingm->setReplicaID(myReplicaID);
                   //Sending the message again
                   send(outgoingm, "outReplicas", i);
                }
            }
            //If all acks has been received for the current message (msgID)
            if (okToSend) {
                //We update the lamport clock because an event is happening with the replica system
                lamportClockHandle(m);
                //Set up the new lamport clock of the message
                m->setLamportClock(lamportClock);
                //Sending to the remote update answer gate, such that the remote write protocol can finish processing the request
                send(m->dup(), "outAnswer", RU_OUT_GATE);
                //We update our last processed lamport clock
                //lcLastMsgSent = m->getLamportClock();
                msgToDelete[msgID] = true;
                //We delete the reference to the local message, because we always have been sent a duplication
                delete m;
            }
        }
        //We delete the messages that already received all the acks
        std::map<long, bool>::iterator it2;
        for (it2 = msgToDelete.begin(); it2 != msgToDelete.end(); it2++)
        {
            bool del = it2->second;
            if(del)
            {
               long msgID =it2->first;
                //We delete the message id from the vector of the messages waiting for acks
               msgsWaitingForAck.erase(msgID);
               //We delete the message id from the vector of the messages to which we need to check the acks state
               msgsAck.erase(msgID);
            }
        }
        //While there are messages waiting for acks, we need to keep having a timer for checking the acks of that mesasge
        if (!msgsWaitingForAck.empty()) {
            if(timeToCheckAcks->isScheduled())
            {
                cancelEvent(timeToCheckAcks);
            }
            scheduleAt(simTime() + exponential(caTimerOffset), timeToCheckAcks);
        }
    }
    // all other time constants that we received any msgs that we don't process
    else {
        SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
        //We update our local lamport clock
        lamportClockHandle(sMsg);
        int msgLamportClk = sMsg->getLamportClock();
        int msgGateID = sMsg->getArrivalGateId();
        int msgClientID = sMsg->getClientID();
        int msgReplicaID = sMsg->getReplicaID();
        int msgReplicaOwnerID = sMsg->getReplicaOwnerID();

        //Messages outgoing from the replica
        if (msgGateID == findGate("inRemoteRequests", RW_IN_GATE) ||
                msgGateID == findGate("inRemoteRequests", RU_IN_GATE) ||
                msgGateID == findGate("inAnswer")) {
            //We set up the the timestamp of the outgoing message
            sMsg->setLamportClock(lamportClock);
            //We save a duplication of the message such that we have the memory control in the current replica
            outQueue.push_back(sMsg->dup());

        }
        //Message comes from a client to a replica
        //TODO: add logic here
        else if ((msgClientID != -1) && (msgReplicaID == -1) && (msgReplicaOwnerID == -1)) {
            inQueue.push_back(sMsg->dup());
            orderInQueue();
        }
        //Messages incoming to the replica
        else {
            // incoming msg has lamport clock ealier than already sent-out-msgs for processing inside
           if (msgLamportClk < lcLastMsgSent) {
               sMsg->setReplyCode(FAIL);
               // this is fail, so put it in the outQueue
               sMsg->setLamportClock(lamportClock);
               //We save a duplication of the message such that we have the memory control in the current replica
               outQueue.push_back(sMsg->dup());
           }
           //incoming msg has lamport clock later than lcLastMsgSent
           else{
                inQueue.push_back(sMsg->dup());
                orderInQueue();
           }
        }
        //We delete the original message from the client
        delete sMsg;
    }
}
//TODO Implement a smarter way of ordering
void ReplicaNetwork::orderInQueue()
{
    bool swapped = true;
    int j = 0;
    SystemMsg* tmp;
    while (swapped) {
        swapped = false;
        j++;
        int inQueueSize = inQueue.size();
        for (int i = 0; i < inQueueSize - j; i++) {
            SystemMsg *currentMsg = inQueue[i];
            SystemMsg *nextMsg = inQueue[i+1];
            if (currentMsg->getLamportClock() > nextMsg->getLamportClock()) {
                tmp = currentMsg;
                currentMsg = nextMsg;
                nextMsg = tmp;
                swapped = true;
            }
        }
    }
}
