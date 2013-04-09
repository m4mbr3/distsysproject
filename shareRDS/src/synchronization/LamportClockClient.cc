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

#include "LamportClockClient.h"
#include "SystemMsg_m.h"

Define_Module(LamportClockClient);

void LamportClockClient::initialize()
{
    //We can see the current value of the parameter on the interface
    WATCH(localClock);
    //Obtaining the current local clock that started as zero
    localClock =par("localClock");

}

void LamportClockClient::handleMessage(cMessage *msg)
{
    //Retrieving the msg just received
    SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
    int gateID = sMsg->getArrivalGateId();
    //The name of the message received!!, IT SHOULD HAVE A NAME SET UP BY THE SENDER
    EV<< "LAMPORT_SYNCH: Received message: " << sMsg->getName();
    //Retrieving the current timestamp of the message
    int rcvLamportClock =sMsg->getLamportClock();
    EV<< "LAMPORT_SYNCH: The msg has timestamp" << rcvLamportClock;
    // updating the local clock of the module usign the received value
    if(rcvLamportClock >localClock)
    {
        localClock = rcvLamportClock +1;
    }
    else
    {
        localClock = localClock + 1;
    }
    //Updating the lamport clock of the msg
    sMsg->setLamportClock(localClock);
    //Sending the msg
    EV<< "LAMPORT_SYNCH: The msg  new timestamp" << localClock;
    if (gateID == gate("in",FROM_BASICNETWORK)->getId()){
        send(sMsg, "out",TO_BASICNETWORK);
    }
    else if(gateID == gate("in", FROM_CLIENTNETWORK)->getId()){
        send(sMsg, "out",TO_CLIENTNETWORK);
    }
}
