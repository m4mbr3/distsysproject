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

#ifndef __SHARERDS_INVOCATIONMANAGER_H_
#define __SHARERDS_INVOCATIONMANAGER_H_

#include <omnetpp.h>

//Input gates
//The constant of the input gate for receiving requests
#define REQUEST_IN_GATE 0
//Input gate for receiving answers from sent requests
#define ANSWER_IN_GATE 1
//out
//Output gate for sending the request to a component that starts processing it
#define REQUEST_OUT_GATE 0
//Output gate for sending answer from a received request
#define ANSWER_OUT_GATE 1
/**
 * Invocation Manager that simulates the invocation of a write/read operation from a client request or a replica
 * request for an update
 */
class InvocationManager : public cSimpleModule
{
    private:
        int entityID;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
