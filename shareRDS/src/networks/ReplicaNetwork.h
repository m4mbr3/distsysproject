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

// offset for timer to count for sending out request
#define TIMER_OFFSET 1

class ReplicaNetwork : public cSimpleModule
{
private:
    int myReplicaID;      //ID of the running replica module
    int lamportClock;         // scalar clock ~ lamportClock

    int lcLastMsgSent;     // timestamp of the last sent msg

    std::map<long, std::vector<bool> > msgsAck;     // queue for acks that are being waited as answers for RU sent out
    std::map<long, SystemMsg*> msgsWaitingForAck;
    std::vector<SystemMsg*> inQueue;
    // queue for requests that are received from outside and in queue for processing
    // those are msgs from Clients, from Replicas requests or Replicas answers

    std::vector<SystemMsg*> outQueue;      // queue for requests that are in queue for sending out

    cMessage *timeToProcessRequest;    // a self msg to trigger queuing process overtime
    cMessage *timeToSendOutRequest;     // a self msg to trigger sending out msg overtime
    cMessage *timeToCheckAcks;

    void lamportClockHandle(SystemMsg *msg);
    void orderInQueue();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
