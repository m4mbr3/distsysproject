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

#include "BasicNetwork.h"

Define_Module(BasicNetwork);

void BasicNetwork:initialize()
{
    // TODO - Generated method body
}

void BasicNetwork::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    int gateID = ttmsg->getArrivalGateId();
    if (gateID == gate("in",FROM_REPLICAGROUPMANAGER)->getId()){
        open_connections.push_back( std::pair<int,int>(ttmsg->getReplicaID(),ttmsg->getDataID()) );
        send(ttmsg, "out"+TO_REPLICAGROUPMANAGER);
    }
    else if ( gateID == gate ("in",FROM_CLIENTNETWORK)->getId()){
        std::find(open_connections.begin(), open_connections.end(), std::pair<int, int>(ttmsg->getReplicaID(),ttmsg->getDataID()));
        printf();
    }

}
