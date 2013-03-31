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

#ifndef __SHARERDS_APPMSGGENERATOR_H_
#define __SHARERDS_APPMSGGENERATOR_H_

#include <omnetpp.h>
#include "SystemMsg_m.h"

/**
 * This class simulates the client/replica generation messages to the replica system
 */
class AppMsgGenerator : public cSimpleModule
{
   private:
   //the client id
    int cID;
    //the total number of replicas
    int nReplicas;
    //The timer for start sending messages to the server, simulating client requests
    cMessage* timeToSendMessage;
    //The replica id to which it is connected
    int replicaID;
    //the current data id
    std::string dataID;
    //local clock
    int localClock;
    //build a message to be send into the network
    cMessage* getMessage();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
