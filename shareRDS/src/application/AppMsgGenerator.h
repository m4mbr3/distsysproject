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

    //The replica id to which it is connected
    int replicaID;
    //Operations order
    std::vector<std::string> operations;
    //Data items ids
    std::vector<std::string> dataItemsId;
    //Counter for counting how many messages have been sent
    int counter;
    //The time for the self message timer
    double timerOffset;
    //the current data id involved in the current message sent by the client
    std::string dataID;
    //local clock
    int localClock;
    //The timer for start sending messages to the server, simulating client requests
    cMessage* timeToSendMessage;
    //build a message to be send into the network
    cMessage* getMessage();

   public:
    //Defining a constructor
    AppMsgGenerator();
    //defining the destructor
    virtual ~AppMsgGenerator();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
