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

#include "ClientNetwork.h"

Define_Module(ClientNetwork);

void ClientNetwork::initialize()
{
    // TODO - Generated method body
}

void ClientNetwork::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    int gateID = ttmsg->getArrivalGateId();
    if (gateID == gate("in",FROM_BASICNETWORK)->getId()){
        //Here I look the ReplicaID of the message and i send it to
        //ReplicaID+offset port where offset is 2
        //the port 0 is dedicated to the lamport module
        //the port 1 is dedicated to the basicNetwork module
        //All the others port are dedicated to the connection
        //with the replica
        send(ttmsg, "out"+(ttmsg->getReplicaID()+2));
    }
    else if (gateID == gate("in",FROM_LAMPORT)->getId()){
       //here i send the received message
       send(ttmsg, "out"+TO_BASICNETWORK);
    }
    else{
        //here it means that ttmsg is sent from one of my connected replica
        //it could be either an answer or an ack
        //At this point i forward it directly to the basic network module
        send(ttmsg, "out"+TO_LAMPORTCLOCK);
    }
}
