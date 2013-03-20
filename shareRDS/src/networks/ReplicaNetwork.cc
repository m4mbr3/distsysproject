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
    replicaID = par("replicaID");
    // Validating that a replica ID was defined
    if(replicaID == -1)
        throw cRuntimeError("Invalid replica ID %d; must be >= 0", replicaID);

    //TODO: create a queue to store msg

}

void ReplicaNetwork::handleMessage(cMessage *msg)
{
    // retrieve the msg and cast to SystemMsg
    SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);

    // retrieve the gateID of the gate from which we received the msg
    int gateID = sMsg->getArrivalGateId();

    // retrieve other data from the msg
    int msgClientID = sMsg->getClientID();
    int msgReplicaID = sMsg->getReplicaID();
    int msgLamportClk = sMsg->getLamportClock();
    int msgReplyCode = sMsg->getReplyCode();
    int msgOperation = sMsg->getOperation();
    std::string msgDataID = sMsg->getDataID();

    // retrieve the treeID of the msg because it won't change when cloning the msg
    int msgTreeID = sMsg->getTreeId();

    /*if (gateID == findGate("inRequests")){
        if (gateID == gate("inRequests", 0)->getId()) {
        // the msg is sent to inRequest[0], which means it comes from RemoteWriteProtocol for remoteWrite
            //TODO: do we need to check if the msg is on top of the queue or not?
            send(sMsg, gate("outReplicas", msgReplicaID));
        }
        else if (gateID == gate("inRequests", 1)->getId()) {
        // the msg is sent to inRequest[1], which means it comes from RemoteWriteProtocol for remoteUpdate
            //TODO: do we need to check if the msg is on top of the queue or not?
            // forward the msg to ALL replicas
            for (int i = 0; i < gateSize("outReplicas"); i++) {
                send(sMsg, gate("outReplicas", i));
            }
        }
    }
    // the msg is send to one of inClients gates, which means it comes from a client
    else if (gateID == findGate("inClients")) {
        // iterate through all inClients gates:
        for (int i = 0; i < gateSize("inClients"); i++){
            // to find the sender client
            if (gateID == findGate("inClients", i)) {
                // check the operation of the msg:

                // READ request from a client
                if (msgOperation == READ) {
                    // boolean flag to mark if the READ request is younger than
                    bool isYoung = true;
                    // iterate from the last item in queue
                    for (int j = queuingMsg.size(); j >= 0 ; --j) {
                        // compare with msg on the same dataID
                        if (msgDataID == queuingMsg.at(j).getDataID()) {
                            // incoming msg is older than a WRITE in queue
                            if ((msgLamportClk < queuingMsg.at(j).getLamportClock()) && (queuingMsg.at(j).getOperation() == WRITE)) {
                                //TODO reject because the READ request is older than existing WRITE in queue
                                isYoung = false;
                                sMsg->setReplyCode(FAIL);
                                send(sMsg, gate("outClients", i));
                                break;  // a break here will interrupt the FOR iteration?
                            }
                        }
                    }
                    // incoming READ request is younger than any WRITE in queue
                    if (isYoung) {


                    }
                }
                else if (msgOperation == WRITE) {
                // WRITE request from a client
                }
                //TODO: don't know what to do next =)))
            }
        }
    }
    else if (gateID == findGate("inReplicas")){
    // the msg is sent to one of inReplicas gates, which means it comes from another replica
        // iterate through all inReplicas gates:
        for (int i = 0; i < gateSize("inReplicas"); i++) {
            // to find the correct replica
            if (gateID == findGate("inReplicas", i)) {

            }
        }
    }
    else if (gateID == findGate("inAnswer")) {
    // the msg is sent to inAnswer gate, which means it is a confirmation msg with replyCode
    }
    else if (gateID == findGate("inLamportClk")) {
    // the msg is sent to inLamportClk
    // I don't know what it means yet :)))))
    }*/

    //TODO START EVERYTHING AGAIN 20/3/2013

    // iterate through all inClients gates
    for (int i = 0; i < gateSize("inClients"); i++) {
        // if incoming msg is from one of the inClients gate:
        if (gateID == findGate("inClients", i)) {
        // then check the msg operation
            // msg operation is WRITE request from a client
            if (msgOperation == WRITE) {
                // send the msg to Lamport clock to set new lamport clock
                send(sMsg, "outLamportNew");
            }
            // msg operation is READ request from a client
            else if (msgOperation == READ) {
                // send the msg to Lamport clock to set new lamport clock
                send(sMsg, "outLamportNew");
            }
        }
    }
    // incoming msg is from inRequests[0]
    // RemoteWrite from RemoteWriteProtocol
    if (gateID == findGate("inRequests", 0)) {
        //
    }
}
