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

#ifndef __SHARERDS_REMOTEWRITEPROTOCOL_H_
#define __SHARERDS_REMOTEWRITEPROTOCOL_H_

#include <omnetpp.h>

/**
 * CONSTANTS
 */
//This constant represent the replica ID when it is -1, in means that the received message is not coming from a replica
#define NO_REPLICA -1

/**
 * Remote write protocol implementation
 */
class RemoteWriteProtocol : public cSimpleModule
{
    private:
    //The replica ID there the module is running
    int replicaID;
    //The map that maintains the relation of the owned data items (the replica is the primary server of these data items)
    std::map<std::string,bool> ownedDataItems;
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
