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

#ifndef __SHARERDS_CLIENTREINCARNATION_H_
#define __SHARERDS_CLIENTREINCARNATION_H_

#include <omnetpp.h>
#include <string>
#include "FailureManager.h"
#include "../replication/DataItemsManager.h"
/**
 * TODO - Generated class
 */
class ClientReincarnation : public cSimpleModule
{

//    public:
//        int clientID;
//        std::map<std::string,int> ownedDataItems;

        //get and set methods
//        virtual int getClientID();
//        virtual void setClientID(int clientID);
//        virtual int getDataItem(std::string dataID);
//        virtual void setDataItem(int dataValue);
//        //Class costructor
//        virtual ClientReincarnation(String clientID);

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);


};

#endif
