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

#ifndef __SHARERDS_BASICNETWORK_H_
#define __SHARERDS_BASICNETWORK_H_

#include <omnetpp.h>
#include "SystemMsg_m.h"
#define FROM_REPLICAGROUPMANAGER 0
#define FROM_CLIENTNETWORK 1
#define TO_REPLICAGROUPMANAGER 0
#define TO_REPLICAGROUPMANAGER 1
/**
 * TODO - Generated class
 */
class BasicNetwork : public cSimpleModule {
  public:
    std::vector<int,int> open_connections;
    std::vector<int>ReplicaIDs;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
