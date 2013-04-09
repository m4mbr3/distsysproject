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

#include "LamportClock.h"
#include "SystemMsg_m.h"

Define_Module(LamportClock);

void LamportClock::initialize()
{
    //We can see the current value of the parameter on the interface
    WATCH(localClock);
    //Obtaining the current local clock that started as zero
    localClock =par("localClock");

}

void LamportClock::handleMessage(cMessage *msg)
{
    //Retrieving the msg just received
    SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
    //Retrieving the current timestamp of the message
    int rcvLamportClock =sMsg->getLamportClock();
    // updating the local clock of the module usign the received value
    if(rcvLamportClock >localClock)
    {
        localClock = rcvLamportClock +1;
    }
    else
    {
        localClock = localClock + 1;
    }
    // if the msg is coming from inNew gate then we also need to update the lamport clock of the msg
    if (sMsg->getArrivalGateId() == findGate("inNew")){
        //Updating the lamport clock of the msg
        sMsg->setLamportClock(localClock);
    }
    //Sending the msg
    send(sMsg, "out");

}
