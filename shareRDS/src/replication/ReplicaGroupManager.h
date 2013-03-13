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
    private:
    //The number of replicas available (all the replicas from 0 to nReplicas are available)
    int nReplicas;
    //The replica ID in which the replica group manager is installed
    int replicaID;

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
};

#endif
