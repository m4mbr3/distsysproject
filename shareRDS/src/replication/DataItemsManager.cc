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

#include "DataItemsManager.h"
#include "SystemMsg_m.h"




Define_Module(DataItemsManager);

void DataItemsManager::initialize()
{
    //The replica ID where this manager belongs to
    replicaID = par("replicaID");
    //Validating that a replica ID was defined
    if(replicaID == -1)
        throw cRuntimeError("Invalid replica ID %d; must be >= 0", replicaID);
}

void DataItemsManager::handleMessage(cMessage *msg)
{
    //We retrieve the msg
    SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
    //Checking which kind of operation is needed to be executed
    int operation = sMsg->getOperation();
    //Retrieving the data ID sent in the message
     std::string msgDataID = sMsg->getDataID();
    //5.Setting up the answer in the message if the OPERATION is READING
    if(operation == READ)
    {
        //5.1 by default we do not have the data
        int data= NO_DATA;
        //5.2 Retrieving the data identified by the given ID
        try{
            data = dataItems.at(msgDataID);
            //putting the data on the answer msg
            sMsg->setData(data);
        }
        //Something strange happens, because the data item does not exists
        catch (const std::out_of_range& e)
        {
            throw cRuntimeError("DATAITEMS_MANAGER:(1) the data item wit id %s does not exists in replica id %d",sMsg->getDataID(), replicaID);
        }
    }
    //Setting up the answer in the message if the OPERATION is WRITING
    else if(operation == WRITE)
    {
        //Retrieving the data sent on the message
        int data = sMsg->getData();
        dataItems[msgDataID] = data;

    }
    //Setting up the answer in the message if the OPERATION is DELETING
    else if(operation == DELETE){

        dataItems.erase(msgDataID);

    }
    //We always answer a success answer because the Replica do not fail, so the operation require must execute successfully
     sMsg->setReplyCode(SUCCESS);
    //Sending the message to the dist. mutual exclusion component
    send(sMsg,"out");

}
