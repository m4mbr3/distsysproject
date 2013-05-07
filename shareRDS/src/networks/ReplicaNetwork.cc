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


//TODO: need to consider the lamport clock for the order of the messages



#include "ReplicaNetwork.h"
#include "SystemMsg_m.h"
#include <vector>

Define_Module(ReplicaNetwork);

void ReplicaNetwork::initialize()
{
    // Initialize the replica ID of the replica that has the group manager module
    myReplicaID = par("replicaID");
    // Validating that a replica ID was defined
    if(myReplicaID == -1)
        throw cRuntimeError("Invalid replica ID %d; must be >= 0", myReplicaID);

    lamportClock = 0;
    WATCH(lamportClock);

    lcLastMsgSent = -1;

    timeToProcessRequest = new cMessage("processRequest");

    // schedule to send msg to request to process queuing msg
    // send this to InvocationManager
    scheduleAt(simTime() + 1, timeToProcessRequest);

    timeToSendOutRequest = new cMessage("sendOutRequest");
    // schedule to process outQueue
    scheduleAt(simTime() + 2, timeToSendOutRequest);

    timeToCheckAcks = new cMessage("checkAcks");
    scheduleAt(simTime() + 3, timeToCheckAcks);

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

    // DONE inQueue-----------------------------------------------------
    if (msg == timeToProcessRequest) {
        int size = inQueue.size();
        for (int i = 0; i < size; i++) {
            SystemMsg* sMsg = inQueue.at(i);
            // retrieve the gateID of the gate from which we received the msg
            int gateID = sMsg->getArrivalGateId();
            // retrieve other data from the msg
            int msgClientID = sMsg->getClientID();
            int msgReplicaID = sMsg->getReplicaID();
//            int msgReplicaOwnerID = sMsg->getReplicaOwnerID();
//            int msgLamportClk = sMsg->getLamportClock();
            int msgReplyCode = sMsg->getReplyCode();
            int msgOperation = sMsg->getOperation();
            std::string msgDataID = sMsg->getDataID();

            // retrieve the treeID of the msg because it won't change when cloning the msg
            int msgTreeID = sMsg->getTreeId();

            // if the incoming msg is an ACK answer of a RemoteUpdate from another replica
            if ((gateID == findGate("inReplicas", myReplicaID)) && (msgOperation == ACK)) {
                std::vector<bool> inAck;   // a vector to store all ACKs received for this msg
                try {
                    //TODO: check if we need to use * or & or not
                    inAck = msgsAck.at(msgTreeID);
                    inAck[msgReplicaID] = true;

                } catch (const std::out_of_range& e) {
                    //TODO: do something
                }
            }
            // incoming msg is an answer for a RemoteWrite request
            else if ((gateID == findGate("inReplicas", msgReplicaID)) && (msgReplyCode != -1)) {
                // check if the msg is on top of the outQueue or not
                // if it's on top then send
                send(msg, "outAnswer", RW_OUT_GATE);
                lcLastMsgSent = lamportClock;
            }
            // incoming msg is from a Client
            else if (gateID == findGate("inClients", msgClientID)){
                send(msg, "outRequest");
                lcLastMsgSent = lamportClock;
            }
        }
        //we schedule again the processing timer
        scheduleAt(simTime() + exponential(10.0),timeToProcessRequest);
    }
    // DONE outQueue-----------------------------------------------------
    else if (msg == timeToSendOutRequest) {
        int size = outQueue.size();
        for (int i = 0; i < size; i++) {
            SystemMsg* sMsg = outQueue.at(i);


            // retrieve the gateID of the gate from which we received the msg
//            int gateID = sMsg->getArrivalGateId();

            // retrieve other data from the msg
            int msgClientID = sMsg->getClientID();
            int msgReplicaID = sMsg->getReplicaID();
            int msgReplicaOwnerID = sMsg->getReplicaOwnerID();
//            int msgLamportClk = sMsg->getLamportClock();
            int msgReplyCode = sMsg->getReplyCode();
//            int msgOperation = sMsg->getOperation();
            std::string msgDataID = sMsg->getDataID();

            sMsg->setReplicaID(myReplicaID);

            // retrieve the treeID of the msg because it won't change when cloning the msg
            int msgTreeID = sMsg->getTreeId();

            if (msgReplicaID == -1) {
                send(msg, "outClients", msgClientID);
            }
            // incoming msg is an answer of a RemoteUpdate request, we need to send it to replicaOwnerID
            else if (msgReplicaOwnerID != myReplicaID) {
                sMsg->setOperation(ACK);
                send(msg, "outReplicas", msgReplicaOwnerID);
            }

            else if (msgReplicaOwnerID == myReplicaID) {
                // multicast
                if (msgReplyCode == -1) {

                    msgsWaitingForAck[msgTreeID] = sMsg;
                    int noOfReplicas = gateSize("outReplicas");
                    std::vector<bool> temp(noOfReplicas);
                    for (int i = 0; i < noOfReplicas; i++) {
                        temp[i] = false;
                    }
                    msgsAck[msgTreeID] = temp;

                    for (int i = 0; gateSize("outReplicas"); i++) {
                        SystemMsg* outgoingMsg = sMsg->dup();
                        send(outgoingMsg, "outReplicas",i);
                    }

                    scheduleAt(simTime() + exponential(10.0), timeToCheckAcks);
                }
                // incoming msg is an answer of a RemoteWrite request, we need to send it to the sender
                else {
                    send(msg, "outReplicas", msgReplicaID);
                }
            }
        }

        scheduleAt(simTime() + exponential(10.0), timeToSendOutRequest);
    }


    else if (msg == timeToCheckAcks) {
        std::map<long, std::vector<bool> >::iterator it;
        for (it == msgsAck.begin(); it != msgsAck.end(); it++) {
            int msgID = it->first;
            bool okToSend = true;
            int size = (it->second).size();
            for (int i = 0; i < size; i++) {
                // check if we already received the ACK or not
                if (!(it->second)[i]) { // false
                    okToSend = false;
                    send(msgsWaitingForAck[msgID], "outReplicas", i);
                }
            }
            if (okToSend) {
                send(msgsWaitingForAck[msgID], "outAnswers", RU_OUT_GATE);
                lcLastMsgSent = lamportClock;
                msgsWaitingForAck.erase(msgID);
                msgsAck.erase(msgID);
            }
        }

        if (!msgsWaitingForAck.empty()) {
            scheduleAt(simTime() + exponential(10.0), timeToCheckAcks);
        }
    }

    // all other time constants that we received any msgs that we don't process
    else {
        SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
        lamportClockHandle(sMsg);
        //
//        SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
        int msgLamportClk = sMsg->getLamportClock();
//        int msgReplicaID = sMsg->getReplicaID();
//        int msgClientID = sMsg->getClientID();
        int msgGateID = sMsg->getArrivalGateId();

        // incoming msg has lamport clock ealier than already sent-out-msgs for processing inside
        if (msgLamportClk < lcLastMsgSent) {
            sMsg->setReplyCode(FAIL);
            // this is fail, so put it in the outQueue
            sMsg->setLamportClock(lamportClock);
            outQueue.push_back(sMsg);
        }
        // incoming msg has lamport clock later than lcLastMsgSent
        else {
            if ((msgGateID == findGate("inRequests", RW_IN_GATE)) ||
                    (msgGateID == findGate("inRequests", RU_IN_GATE)) ||
                    (msgGateID == findGate("inAnswer"))) {
                sMsg->setLamportClock(lamportClock);
                outQueue.push_back(sMsg);
            }
            else {
                inQueue.push_back(sMsg);
                orderInQueue();
            }
        }
    }
}

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
