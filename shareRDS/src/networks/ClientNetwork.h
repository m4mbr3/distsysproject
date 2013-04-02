// // This program is free software: you can redistribute it and/or modify
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

#ifndef __SHARERDS_CLIENTNETWORK_H_
#define __SHARERDS_CLIENTNETWORK_H_

#include <omnetpp.h>
#include "SystemMsg_m.h"
#define FROM_BASICNETWORK 1
#define TO_BASICNETWORK 1
#define FROM_LAMPORT 0
#define TO_LAMPORT 0

/**
 * TODO - Generated class
 */
class ClientNetwork : public cSimpleModule {
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
