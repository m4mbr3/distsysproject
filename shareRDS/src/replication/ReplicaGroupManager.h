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

#ifndef __SHARERDS_REPLICAGROUPMANAGER_H_
#define __SHARERDS_REPLICAGROUPMANAGER_H_

#include <omnetpp.h>
#include "SystemMsg_m.h"
#define FROM_WRITEAHEADPROTOCOL 0
#define FROM_CLIENTREINCARNATION 1
#define FROM_INVOCATIONMANAGER 2
#define TO_WRITEAHEADPROTOCOL 0
#define TO_CLIENTREAINCARNATION 1
#define TO_NETWORK 2
/**
 * The simple module that is in charge of managing the replicas in the
 * System.
 * ASSUMPTIONS:
 * In this module the Replicas are assume to not fail, therefore all the
 * Replicas are available during the whole time of the execution. In case this assumption disappear, the
 * algorithms for identifying the non faulty processes should be implemented in this module.
 */
class ReplicaGroupManager : public cSimpleModule
{
    public:
        int replicaID;
        int reincCounter;
        bool dead;
        int clientID;
        bool willfail;
        std::vector<int> ReplicaIDs;
        SystemMsg* generateReincarnationMessage(int replica, int clientID);
        int getClientID();
        void setClientID(int clientID);
    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
