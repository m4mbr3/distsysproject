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

#include "InvocationManager.h"
#include "SystemMsg_m.h"

Define_Module(InvocationManager);

void InvocationManager::initialize()
{
    //The replica ID where this manager belongs to
    entityID = par("entityID");
    //Validating that a replica ID was defined
    if(entityID == -1)
        throw cRuntimeError("Invalid ID %d; must be >= 0", entityID);
}

void InvocationManager::handleMessage(cMessage *msg)
{
    //We retrieve the msg
    SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
    //We retrieve the gate of arrival
    int gateID = sMsg->getArrivalGateId();
    //if we receive a request
    if(gateID == gate("in", REQUEST_IN_GATE)->getId())
    {
        //We send the message to the component that should start processing the request
        send(msg, "out", REQUEST_OUT_GATE);
    }
    //if we have receive an answer from a request sent in the past of one middle step when processing a request
    else if(gateID == gate("in", ANSWER_IN_GATE)->getId())
    {
        //We send back the answer to the component that has sent the invocation to which we already have the answer
         send(msg, "out", ANSWER_OUT_GATE);
    }
}
