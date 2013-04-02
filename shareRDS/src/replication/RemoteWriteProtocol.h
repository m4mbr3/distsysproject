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
//The message is a request message
#define NO_REPLY_CODE -1
/**
 * Output gates
 */
//Output gate to the invocation manager
#define IM_OUT_GATE 0
//Output gate to the data items manager
#define DIM_OUT_GATE 1
//Output gate for logging the writing message on a data item
#define WAP_OUT_GATE 2
//Output gate to remote replica (remote write)
#define RW_OUT_GATE 3
//Output gate for sending an update of a owned data item
#define RU_OUT_GATE 4

/**
 * Input gates
 */
//Input gate from the invocation manager, representing a client/replica connected to the current replica
#define IM_IN_GATE 0
//Input gate for receiving answers from read/write operations executed by the data items manager
#define DIM_IN_GATE 1
//Input gate from the write ahead protocol component for logging a write action on a data item
#define WAP_IN_GATE 2
//Input gate for receiving answer of remote write to a replica that owns the data item involved in the protocol
#define RW_IN_GATE 3
//Input gate for receiving updates operations from the other replicas
#define RU_IN_GATE 4

/**
 * Remote write protocol implementation
 */
class RemoteWriteProtocol : public cSimpleModule
{
    private:
    //The replica ID there the module is running
    int replicaID;
    //An attribute that allows to know if the replica owns the data item involved in the request
    bool ownDataItem;
    //The map contains the information related to ownership of a data item in the replicas system
    std::map<std::string,int> dataItemsOwners;
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
