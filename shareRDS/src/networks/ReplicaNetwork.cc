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
    timeToCheckRemoteRequests = NULL;
    timeToCheckTTL = NULL;
    multicastMaxRetries = -1;
    remoteRequestMaxRetries =-1;
    lamportClock = 0;
    lcLastMsgSent = lamportClock;
    ttlTimerOffset = -1;
    ttlWindow = -1;
}

ReplicaNetwork::~ReplicaNetwork()
{
    //Cleaning up the maps
    msgsAck.clear();
    //Cleaning up the messages that are waiting for acks
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
    //Clean the remote write requests waiting for answer
    remoteRequests.clear();
    remoteReqState.clear();
    //Cancelling and deleting the timers
    cancelAndDelete(timeToSendOutRequest);
    cancelAndDelete(timeToProcessRequest);
    cancelAndDelete(timeToCheckAcks);
    cancelAndDelete(timeToCheckRemoteRequests);
    cancelAndDelete(timeToCheckTTL);
}

void ReplicaNetwork::initialize()
{
    // Initialize the replica ID of the replica that has the group manager module
    myReplicaID = par("replicaID");
    // Validating that a replica ID was defined
    if(myReplicaID == -1)
        throw cRuntimeError("REPLICA NETWORK: Invalid replica ID %d; must be >= 0", myReplicaID);
    //Initializing the lamport clock
    lamportClock = 0;
    WATCH(lamportClock);

    //Initializing the timestamp (using the lamport clock) for the last message processed in the system
    lcLastMsgSent = -1;
    WATCH(lcLastMsgSent);

    //Configuring the timer for processing the input queue
    timeToProcessRequest = new cMessage("processingRequestTimer");
    pTimerOffset = par("processingTimerOffset").doubleValue();
    scheduleAt(simTime() + exponential(pTimerOffset), timeToProcessRequest);

    //Configuring the timer for processing the output queue
    sTimerOffset = par("sendingTimerOffset").doubleValue();
    timeToSendOutRequest = new cMessage("sendRequestTimer");
    scheduleAt(simTime() + exponential(sTimerOffset), timeToSendOutRequest);

    //Configuring the timer for checking acks and then use it when necessary
    caTimerOffset =  par("checkAckTimerOffset").doubleValue();
    timeToCheckAcks = new cMessage("checkAcksTimer");

    //Configuring the timer for checking the state of a remote write request
    crrTimerOffset = par("checkRemoteReqTimerOffset").doubleValue();
    //We create the self message (timer) for checking the stat of a remote requests
    timeToCheckRemoteRequests = new cMessage("checkRemoteReqTimer");

    //Configuring the timer for checking ttl of a msg
    ttlTimerOffset = par("checkMsgTTL").doubleValue();
    // Create the self msg (timer) for checking the ttl of a msg
    timeToCheckTTL = new cMessage("checkTTLTimer");
    // initialize the window size
    ttlWindow = par("ttlWindow");
    scheduleAt(simTime() + exponential(ttlTimerOffset), timeToCheckTTL);

    //Recovering the maximum number of retries for multicast and remote request messages
    multicastMaxRetries = par("multicastMaxTries");
    remoteRequestMaxRetries = par("remoteRequestMaxTries");
    //We watch them
    WATCH(multicastMaxRetries);
    WATCH(remoteRequestMaxRetries);

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

bool ReplicaNetwork::searchForMsgInQueue (int clientID,int replicaID, int replicaOwnerID, int replyCode, int operation, std::string dataID, int data, int queueType, int msgTreeId)
{
    bool result = false;

    // rebuild a new systemMsg
    SystemMsg* temp = new SystemMsg("temp");
    temp->setClientID(clientID);
    temp->setReplicaID(replicaID);
    temp->setReplicaOwnerID(replicaOwnerID);
    temp->setReplyCode(replyCode);
    temp->setOperation(operation);
    temp->setDataID(dataID.c_str());
    temp->setData(data);

    std::vector<SystemMsg*>::iterator it;
    SystemMsg* m = NULL;

    switch (queueType) {
    case INPUT_QUEUE:
        for (it = inQueue.begin(); it != inQueue.end() && !result; it++) {

            m = *it;
            EV << "[" << myReplicaID << "] inputQueue data: (treeID:"<<m->getTreeId() << ",clientID:" << m->getClientID() << ",replicaID:" << m->getReplicaID() << ",replyCode:" << m->getReplyCode() << ",ownerRepID:" << m->getReplicaOwnerID() << ",op:" << m->getOperation() << ")\n";

            if (m->getTreeId() != msgTreeId)
                continue;
            else {
                result = compareMsgs(m, temp);
            }
        }
        break;
    case OUTPUT_QUEUE:
        for (it = outQueue.begin(); it != outQueue.end(); it++) {
            m = *it;
            if (m->getTreeId() != msgTreeId)
                continue;
            else {
                result = compareMsgs(m, temp);
            }
        }
        break;
    case AUXMAP:
        try {
            m = auxMap.at(msgTreeId);
            bool foundMsg = compareMsgs(m, temp);
            if (foundMsg) {
                int timerOffset = m->getLamportClock() + ttlWindow - lamportClock;
                if (timerOffset >= 0) {
                    result = false;
                }
                else {
                    delete m;
                    auxMap.erase(msgTreeId);
                    result = true;
                }
            }
        } catch (std::exception& e) {
        }
        break;
    default:
        break;
   }
    delete temp;
    return result;

}

bool ReplicaNetwork::compareMsgs(SystemMsg* m1, SystemMsg* m2) {
    bool areEqual = true;
    if (m1->getClientID() != m2->getClientID())
        areEqual = false;
    if (m1->getReplicaID() != m2->getReplicaID())
        areEqual = false;
    if(m1->getReplicaOwnerID() != m2->getReplicaOwnerID())
        areEqual = false;
    if (m1->getReplyCode() != m2->getReplyCode())
        areEqual = false;
    if (m1->getOperation() != m2->getOperation())
        areEqual = false;
    std::string s1 = m1->getDataID();
    std::string s2 = m2->getDataID();
    if (s1.compare(s2) != 0)
        areEqual = false;
    if(m1->getData() != m2->getData())
        areEqual = false;
//    if (m1->getTreeId() != m2->getTreeId())
//        areEqual = false;

    return areEqual;
}

void ReplicaNetwork::handleMessage(cMessage *msg)
{

    /**************************************************************************************
     * Time to start processing the messages saved in the inQueue
     **************************************************************************************/
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

            if (msgOperation != ACK) {
                delete auxMap[msgTreeID];
                // save a duplicate of the msg into the auxMap
                EV << "[" << myReplicaID << "] processing (treeID:"<<msgTreeID << ",clientID:" << msgClientID << ",replicaID:" << msgReplicaID << ",replyCode:" << msgReplyCode << ",ownerRepID:" << msgOwnerReplicaID << ",op:" << msgOperation << ")\n";
                auxMap[msgTreeID] = sMsg->dup();

            }

            // the incoming msg from another replica and it is an ACK answer of a RemoteUpdate from the current replica
            if ((gateID == findGate("inReplicas", msgReplicaID)) && (msgOperation == ACK)) {
                bubble("Received an ACK, checking it");
                // first we check whether if we are waiting for this ACK or not
                // by looking for the same msgTreeID in waiting for ACK queue
                std::map<long, SystemMsg*>::iterator it;
                bool delThisAck = true;
                try {
                    for (it = msgsWaitingForAck.begin(); it != msgsWaitingForAck.end(); it++) {
                        if (msgTreeID == it->first) {
                            delThisAck = false;     // if we find the same msgTreeID in the queue then won't delete this ACK
                        }
                    }
                } catch (const std::exception& e) {
                    throw cRuntimeError("Cannot read msgsWaitingForAck, it might be empty");
                }

//
//                // after the checking iteration, if we cannot find the same msgTreeID in the queue
//                // then the received ACK is unknown, then DELETE it
                if (delThisAck) {
                    bubble("Received unknown or already processed ACK!");
                    delete sMsg;
                }
//                // otherwise, process it
                else {
                    bubble("Legal ACK, processing it");
                    std::vector<bool> inAck;   // a vector to store all ACKs received for this msg
                    try {
//                        inAck = msgsAck.at(msgTreeID);
//                        inAck[msgReplicaID] = true;
//                        msgsAck[msgTreeID]=inAck;
                        msgsAck[msgTreeID][msgReplicaID]=true;
                        delete sMsg;
                    } catch (const std::out_of_range& e) {
//                        throw cRuntimeError("some error here");
                        delete sMsg;
                    }

                }
            }
            // incoming msg is an answer for a RemoteWrite request that the current replica has request in a remote write request
            else if (msgReplicaID!= NO_REPLICA && (gateID == findGate("inReplicas", msgReplicaID)) && msgReplicaID == msgOwnerReplicaID && msgReplyCode == SUCCESS) {
                bubble("answer for a RemoteWrite");
                // check if the msg is on top of the outQueue or not
                // if it's on top then send
                send(sMsg->dup(), "outAnswer", RW_OUT_GATE);
                //We update the timestamp of the last message processed
                lcLastMsgSent = sMsg->getLamportClock();
                ///We delete the reference to the message
                //inQueue.erase(inQueue.begin()+i);
                //We delete the message
                delete sMsg;
                /*
                 * Managing the channel failure for remote requests
                 */
                //We have an answer of a previous remote write request
                remoteReqState[msgTreeID] = true;

            }
            //The incoming msg is a remote update request from any of the replicas
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


    /**************************************************************************************
    * Time to start processing the requests that are in the outqueue
    **************************************************************************************/
    else if (msg == timeToSendOutRequest) {
        int size = outQueue.size();
        for (int i = 0; i < size; i++) {
            //We retrieve a message in the queue
            SystemMsg* sMsg = outQueue[i];
            /**
             * We retrieve other data from the msg
             */
            int msgClientID = sMsg->getClientID();
            int msgReplicaID = sMsg->getReplicaID();
            int msgReplicaOwnerID = sMsg->getReplicaOwnerID();
            int msgReplyCode = sMsg->getReplyCode();
            int gateID= sMsg->getArrivalGateId();
            std::string msgDataID = sMsg->getDataID();
            // retrieve the treeID of the msg because it won't change when cloning the msg
            int msgTreeID = sMsg->getTreeId();

            /**
             * Setting up the message with data from the sender
             */
            //We set up the sender of the message
            sMsg->setReplicaID(myReplicaID);
            //We set up the lamport clock of the message  as the current lamport clock of the replica
            sMsg->setLamportClock(lamportClock);

            /**
             * Processing the out queue by choosing the correct out channel
             */
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
                    //We start the number of retries for the involved message
                    multicastRetries[msgTreeID] =0;
                    /**
                     * Multicast
                     */
                    bubble("Multicasting!");
                    //We multicast the messages, including ourselves
                    for (int i = 0; i< noOfReplicas; i++) {
                        //We send a duplicate of the current message
                        SystemMsg* outgoingMsg = sMsg->dup();
                        //We set up the reply code as -1, because we are waiting for an answer from a remote replica
                        outgoingMsg->setReplyCode(NO_REPLY_CODE);
                        //Updating with the lamport clock
                        //outgoingMsg->setLamportClock(lamportClock);
                        //Changing the color of the multicast msgs
                        outgoingMsg->setKind(4);
                        //Sending out the msg
                        send(outgoingMsg, "outReplicas",i);
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
//               // free the map
//               delete auxMap[msgTreeID];
//               // put the msg into the map
//               auxMap[msgTreeID] = sMsg->dup();
               //We send the msg to the owner of the data item
                send(sMsg->dup(), "outReplicas", msgReplicaOwnerID);
               /*
                * Managing the channel failure for remote requests
                */
                //We save the related remote request
                remoteRequests[msgTreeID] = sMsg->dup();
                //We save the state as not answered
                remoteReqState[msgTreeID] = false;
                //We start the number of retries in zero
                remoteRequestRetries[msgTreeID] =  0;
                //Scheduling the timer
                if (timeToCheckRemoteRequests->isScheduled()) {
                    cancelEvent(timeToCheckRemoteRequests);
                }
                else {
                    scheduleAt(simTime() + exponential(crrTimerOffset), timeToCheckRemoteRequests);
                }
           }
            //incoming msg is an answer of a RemoteWrite request from a remote replica, we need to send it to the sender
            else if (msgReplicaID!=NO_REPLICA && msgReplicaID != myReplicaID && msgReplicaOwnerID == myReplicaID && gateID == findGate("inAnswer")){
//                // free the map
//                delete auxMap[msgTreeID];
//                // put the msg into the map
//                auxMap[msgTreeID] = sMsg->dup();
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
//                // free the map
//                delete auxMap[msgTreeID];
//                // put the msg into the map
//                auxMap[msgTreeID] = outgoingMsg->dup();
                //We send the msg to the owner of the data item that has requested the update of the variable
                send(outgoingMsg, "outReplicas", msgReplicaOwnerID);
            }
            //The msg is an answer of a RemoteUpdate request from the current replica to itself
            //In fact the replicaOwner on the message SHOULD be the same as the replica sender
            else if (msgReplicaID!=NO_REPLICA && msgReplicaID == myReplicaID && msgReplicaOwnerID == myReplicaID  && gateID == findGate("inAnswer") && msgReplyCode ==SUCCESS) {
                //We set up the operation as an ACK
                sMsg->setOperation(ACK);
                //We send a duplicate of the current message
                SystemMsg* outgoingMsg = sMsg->dup();
                // free the map
                //delete auxMap[msgTreeID];
                // put the msg into the map
                //auxMap[msgTreeID] = outgoingMsg->dup();
                //We send the msg to the owner of the data item that has requested the update of the variable
                send(outgoingMsg, "outReplicas", msgReplicaOwnerID);
            }
            //We are processing a message that is an answer to a client request
           else {
               bubble("OUT TO CLIENT!");
//               // free the map
//               delete auxMap[msgTreeID];
//               // put the msg into the map
//               auxMap[msgTreeID] = sMsg->dup();
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


    /**************************************************************************************
    * Checking the acks of a message
    **************************************************************************************/
    else if (msg == timeToCheckAcks) {
        std::map<long, bool> msgToDelete;
        std::map<long, std::vector<bool> >::iterator it;
        for (it = msgsAck.begin(); it != msgsAck.end(); it++) {
            //The msg id of the msg for which we are waiting acks
            int msgID = it->first;
            //By default we dont need to delete this message becasue we do not know if it already has received all the acks
            msgToDelete[msgID] = false;
            //The pointer to the message
            SystemMsg* m = msgsWaitingForAck[msgID];
            /**
             * Retrying the acks not received
             * Note: The retries are NOT done by replica, i.e for each replica we dont retry the maximum
             * number of tries. Instead we only count the retries on the missing acks, i.e that the maximum
             * number of tries is equal to the number of tries we re-send a set of acks which are not received
             * in an ack checking.
             */
            //Validating the retries of the current message
            int nRetries  = multicastRetries[msgID];
           //We have exhausted the number of tries
            if(nRetries >= multicastMaxRetries)
            {
              //We set up the reply code
              SystemMsg* outgoingmsg = m->dup();
              //The multicast has failed
              outgoingmsg->setReplyCode(FAIL);
              //Sending to the remote update answer gate, such that the remote write protocol can finish processing the request
              send(outgoingmsg, "outAnswer", RU_OUT_GATE);
              //We need to mark the message for deleting it from the messages waiting for acks
              msgToDelete[msgID] = true;
              //We delete the reference to the local message, because we always have been sent a duplication
              delete m;
            }
            //We re-try
            else{
                // Retrieving the state of the acks of the current msg
                std::vector<bool> acks = it->second;
                //For validating if we already have ALL acks from all the replicas in the system
                bool okToSend = true;
                //The size of the acks
                int size = acks.size();
                //We check if we have received all the acks of the current message (msgID)
                for (int i = 0; i < size; i++) {
                    //Retrieving the state of the ack on the replica i
                    bool acki = acks[i];
                    // check if we already received the ACK or not, if not then we resend the reques
                    if (!acki) { // false
                        // we should check that the missing ACK is not in the input queue
                        bool ackInInputQueue = searchForMsgInQueue(m->getClientID(),i,myReplicaID, m->getReplyCode(), ACK, m->getDataID(), m->getData(), INPUT_QUEUE,msgID);
                        //TODO:
                        // test if we need to check outputQueue also because in that case, it means that we send the retry too soon

                        if (ackInInputQueue) {
                            continue;
                        }

                       okToSend = false;
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
                    //Sending to the remote update answer gate, such that the remote write protocol can finish processing the request
                    send(m->dup(), "outAnswer", RU_OUT_GATE);
                    //We update our last processed lamport clock
                    //lcLastMsgSent = m->getLamportClock();
                    msgToDelete[msgID] = true;
                    //We delete the reference to the local message, because we always have been sent a duplication
                    delete m;
                } else {
                  //we updte the retries done
                   multicastRetries[msgID]++;
                }
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
               //We delete the retries
               multicastRetries.erase(msgID);
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

    /**************************************************************************************
    * Checking the state of a remote write request
    ***************************************************************************************/
    else if(msg == timeToCheckRemoteRequests)
    {
        //We go through the state of the remote requests sent by the current replica
        std::map<long, bool>::iterator rmtIt =remoteReqState.begin();
        while (rmtIt != remoteReqState.end())
        {
            //We recover the tree id of the message
            long msgTreeID = rmtIt->first;
            //We check how many retries have been done
            int nRetries  = remoteRequestRetries[msgTreeID];
            //We recover the associated message
            SystemMsg* sMsg = remoteRequests[msgTreeID];
            //Did we received an answer of a remote request?
            bool state = rmtIt->second;
            //We have received an answer of the involved remote write request
            if(state)
            {
                delete sMsg;
                remoteRequests.erase(msgTreeID);
                remoteRequestRetries.erase(msgTreeID);
                remoteReqState.erase(rmtIt++);
            }
            //We havent received an answer then we retry if we havent reached the maximum retries number
            else{
                //We prepare the outgoing message
                 SystemMsg* outgoingmsg = sMsg->dup();
                 //Updating the lamport clock
                 outgoingmsg->setLamportClock(lamportClock);
                //Checking if the maximum retries have happened
                if(nRetries >= remoteRequestMaxRetries)
                {
                    //We recover the client involved on the message
                    int msgClientID = sMsg->getClientID();

                    outgoingmsg->setReplyCode(FAIL);
                    //We send a failure message to the client
                    send(outgoingmsg, "outClients", msgClientID);
                    //We cleant our maps
                    delete sMsg;
                    remoteRequests.erase(msgTreeID);
                    remoteRequestRetries.erase(msgTreeID);
                    remoteReqState.erase(rmtIt++);
                }
                //We have still more retries
                else{
                    // we need to check if the ack answer is in the inputQueue or not
                    bool inQueueFlag = searchForMsgInQueue(sMsg->getClientID(),sMsg->getReplicaID(),sMsg->getReplicaOwnerID(),
                                                        sMsg->getReplyCode(), WRITE, sMsg->getDataID(), sMsg->getData(), INPUT_QUEUE, msgTreeID);
                    //TODO:
                    // test if we need to check outputQueue also because in that case, it means that we send the retry too soon

                    if (inQueueFlag) {
                        continue;
                    }

                    //We recover the owner
                    int msgReplicaOwnerID = sMsg->getReplicaOwnerID();
                    //We prepare the outgoing message
                     outgoingmsg->setKind(6);
                    //We send the msg to the owner of the data item
                     send(outgoingmsg, "outReplicas", msgReplicaOwnerID);
                     //We update our number of retries
                     remoteRequestRetries[msgTreeID]++;
                    //We update our iterator
                    ++rmtIt;
                }

            }
        }
        //If we have more pending remote request writes
        //Scheduling the timer
        if(!remoteRequests.empty()){
            if (timeToCheckRemoteRequests->isScheduled()) {
                cancelEvent(timeToCheckRemoteRequests);
            }
            else {
                scheduleAt(simTime() + exponential(crrTimerOffset), timeToCheckRemoteRequests);
            }
        }

    }


    /**************************************************************************************
     * time to check the processing map for removing the msg with expired ttl
     **************************************************************************************/
    else if (msg == timeToCheckTTL) {
        std::map<long, SystemMsg*>::iterator it =auxMap.begin();
        while(it != auxMap.end()) {
            SystemMsg* m = it->second;
            int lc = m->getLamportClock();
            int liveTime = lc + ttlWindow;
            if (liveTime <= lamportClock) {
                //We cleant our maps
                delete m;
                auxMap.erase(it++);
            }
            else {
                it++;
            }
        }
        //If we have more pending remote request writes
        //Scheduling the timer
        if(!auxMap.empty()){
            if (timeToCheckTTL->isScheduled()) {
                cancelEvent(timeToCheckTTL);
            }
            else {
                scheduleAt(simTime() + exponential(ttlTimerOffset), timeToCheckTTL);
            }
        }
    }


    /**************************************************************************************
    * in the case we are receiving something incoming or msg that should go out
    ***************************************************************************************/
    else {
        // check for bit error rate
//        bool ber;
//        ber = check_and_cast<cPacket*>(msg)->hasBitError();
//        if (ber) {  // the msg has bit error rate, we'll delete it
//            bubble("Msg has bit error!\nDeleting it!");
//            delete msg;
//        }
//        else {  // the msg doesnt have bit error rate, process

            SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);

            //We update our local lamport clock
//            lamportClockHandle(sMsg);
            int msgLamportClk = sMsg->getLamportClock();
            int msgGateID = sMsg->getArrivalGateId();
            int msgClientID = sMsg->getClientID();
            int msgReplicaID = sMsg->getReplicaID();
            int msgReplicaOwnerID = sMsg->getReplicaOwnerID();

            //Messages outgoing from the replica
            if (msgGateID == findGate("inRemoteRequests", RW_IN_GATE) ||
                    msgGateID == findGate("inRemoteRequests", RU_IN_GATE) ||
                    msgGateID == findGate("inAnswer")) {
                //We save a duplication of the message such that we have the memory control in the current replica
                EV << "[" << myReplicaID << "] To outQueue:(treeID:"<<sMsg->getTreeId() << ",clientID:" << msgClientID << ",replicaID:" << msgReplicaID << ",replyCode:" << sMsg->getReplyCode() << ",ownerRepID:" << sMsg->getReplicaOwnerID() << ",op:" << sMsg->getOperation() << ")\n";
                outQueue.push_back(sMsg->dup());

            }
            //Message comes from a client to a replica
            //TODO: add logic here
            else if ((msgClientID != -1) && (msgReplicaID == -1) && (msgReplicaOwnerID == -1)) {
                //We update our local lamport clock because a new msg has arrived to the current replica
                lamportClockHandle(sMsg);
                //We put it on the input queue
                inQueue.push_back(sMsg->dup());
                //We order the queue by the timestamp of the message
                orderInQueue();
            }
            //Messages incoming to the replica
            else {
                //We update our local lamport clock because a new msg has arrived to the current replica
                lamportClockHandle(sMsg);
                // incoming msg has lamport clock ealier than already sent-out-msgs for processing inside
               if (msgLamportClk < lcLastMsgSent) {
                   bubble("Msg lc is to young!");
                   //We set up as fail the message
                   sMsg->setReplyCode(FAIL);
                   // this is fail, so put it in the outQueue
                   sMsg->setLamportClock(lamportClock);
                   EV << "[" << myReplicaID << "] To outQueue:(treeID:"<<sMsg->getTreeId() << ",clientID:" << msgClientID << ",replicaID:" << msgReplicaID << ",replyCode:" << sMsg->getReplyCode() << ",ownerRepID:" << sMsg->getReplicaOwnerID() << ",op:" << sMsg->getOperation() << ")\n";
                   //We save a duplication of the message such that we have the memory control in the current replica
                   outQueue.push_back(sMsg->dup());
               }
               //incoming msg has lamport clock later than lcLastMsgSent and
               //therefore it should be processed inside the replica
               else{
                   bool inInputQueueFlag = searchForMsgInQueue(msgClientID,
                        msgReplicaID, msgReplicaOwnerID, sMsg->getReplyCode(),
                        sMsg->getOperation(), sMsg->getDataID(), sMsg->getData(), INPUT_QUEUE,
                        sMsg->getTreeId());

                   bool inOutputQueueFlag = searchForMsgInQueue(msgClientID,
                                           msgReplicaID, msgReplicaOwnerID, sMsg->getReplyCode(),
                                           sMsg->getOperation(), sMsg->getDataID(), sMsg->getData(), OUTPUT_QUEUE,
                                           sMsg->getTreeId());

                   bool inAuxMapFlag = searchForMsgInQueue(msgClientID,
                                                              msgReplicaID, msgReplicaOwnerID, sMsg->getReplyCode(),
                                                              sMsg->getOperation(), sMsg->getDataID(), sMsg->getData(), AUXMAP,
                                                              sMsg->getTreeId());

                   EV << "[" << myReplicaID << "] MsgTreeId "<< sMsg->getTreeId()<<" : flags: input:" << inInputQueueFlag << ",output:" << inOutputQueueFlag << ",aux:" <<inAuxMapFlag << "\n";

                   if (!inInputQueueFlag && !inOutputQueueFlag && !inAuxMapFlag) {

                       EV << "[" << myReplicaID << "] To inputQueue:(treeID:"<<sMsg->getTreeId() << ",clientID:" << msgClientID << ",replicaID:" << msgReplicaID << ",replyCode:" << sMsg->getReplyCode() << ",ownerRepID:" << sMsg->getReplicaOwnerID() << ",op:" << sMsg->getOperation() << ")\n";
                       inQueue.push_back(sMsg->dup());
                       orderInQueue();
                   }
               }
           }
           //We delete the original message from the client
           delete sMsg;
//        }
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
