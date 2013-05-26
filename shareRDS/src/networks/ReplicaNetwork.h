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

#ifndef __SHARERDS_REPLICANETWORK_H_
#define __SHARERDS_REPLICANETWORK_H_

#include <omnetpp.h>
#include "SystemMsg_m.h"

/*
 * INPUT GATES
 */
// inRemoteWrite
#define RW_IN_GATE 0
// inRemoteUpdate
#define RU_IN_GATE 1
/*
 * OUTPUT GATES
 */
// outRemoteWrite
#define RW_OUT_GATE 0
// outRemoteUpdate
#define RU_OUT_GATE 1

//The default values
#define NO_REPLY_CODE -1
#define NO_REPLICA -1



class ReplicaNetwork : public cSimpleModule
{
    private:
        //ID of the running replica module
        int myReplicaID;
        // scalar clock ~ lamportClock
        int lamportClock;
        // timestamp of the last processed msg on the replica such that we can accept or reject incoming requests
        int lcLastMsgSent;

        //MANAGING THE MULTICAST
        // Queue for managing the acks that are being waited as answers for a remote update sent out from this replica
        std::map<long, std::vector<bool> > msgsAck;
        //The messages for which we should take care of the acks
        std::map<long, SystemMsg*> msgsWaitingForAck;
        //The number of retries that have been done for a multicast message
        std::map<long,int> multicastRetries;
        //The maximum number of retries for multicast messages
        int multicastMaxRetries;

        //MANAGING THE INCOMING AND OUTGOING MESSAGES
        // queue for requests that are received from outside and in queue for processing
        // those are msgs from Clients, from Replicas requests or Replicas answers
        std::vector<SystemMsg*> inQueue;
        // queue for requests that are in queue for sending out
        std::vector<SystemMsg*> outQueue;

        //MANAGING THE REMOTE REQUESTS STATE
        //The remote request from which we are expecting an answer
        std::map <long, SystemMsg*> remoteRequests;
        //The remote request state (if it havent been answer)
        std::map<long, bool> remoteReqState;
        //The number of retries for re-sending a remote write
        std::map<long, int> remoteRequestRetries;
        //The maximum number of retries for multicast messages
        int remoteRequestMaxRetries;

        //SELF MESSAGES
        //A self msg to trigger queuing process overtime
        cMessage *timeToProcessRequest;
        //A self msg to trigger sending out msg overtime
        cMessage *timeToSendOutRequest;
        //A self msg to trigger checking out ack msgs overtime
        cMessage *timeToCheckAcks;
        //A self msg to trigger checking if a remote request has been answer
        cMessage *timeToCheckRemoteRequests;
        //TIMERS OFFSETS
        double pTimerOffset;
        double sTimerOffset;
        double caTimerOffset;
        double crrTimerOffset;
        //Update the local lamport clock of the replica
        void lamportClockHandle(SystemMsg *msg);
        //Order the replica messages in the inQueue
        void orderInQueue();
    public:
        ReplicaNetwork();
        virtual ~ReplicaNetwork();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
