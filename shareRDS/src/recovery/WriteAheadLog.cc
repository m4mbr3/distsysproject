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

#include "WriteAheadLog.h"
#include "SystemMsg_m.h"

Define_Module(WriteAheadLog);

void WriteAheadLog::initialize()
{
    //Initialize the replica ID of the replica that has the group manager module
    replicaID = par("replicaID");
    //Validating that a replica ID was defined
    if(replicaID == -1)
        throw cRuntimeError("Invalid replica ID %d; must be >= 0", replicaID);
}

void WriteAheadLog::handleMessage(cMessage *msg)
{
    //We retrieve the msg
    SystemMsg* sMsg = check_and_cast<SystemMsg*>(msg);
    //retrieving the data ID from the message
    std::string msgDataID = sMsg->getDataID();
    //Checking which kind of operation is involved on the message
    int msgOperationID = sMsg->getOperation();
    //Retrieving the data
    int data = sMsg->getData();
    //Verify if we already have a version of the involved data item
    std::vector<int> versions;
    try{
       //checking if the data item with the given ID already exists in the system
        versions=dataItemsLog.at(msgDataID);

    }
    catch (const std::out_of_range& e)
    {
        //otherwise we create it
        dataItemsLog[msgDataID] = versions;
    }
    //Verifying which kind of operation is being required
   //update
   if(msgOperationID == UPDATE)
   {
       versions.push_back(data);
   }
   //commit
   if(msgOperationID == COMMIT)
   {
       dataItemsState[msgDataID]=true;
   }
   //rollback
   if(msgOperationID == ROLLBACK)
   {
       //we delete the last version
       versions.pop_back();
       //We check if we are rolling back an update of a data item or a creation of the data item
       int size = versions.size();
       if(size>0){
           int oData=versions.back();
           //we send back the old value
           sMsg->setData(oData);
       }
       //is a just created data item
       else
           sMsg->setOperation(DELETE);

   }
   sMsg->setReplyCode(SUCCESS);
   send(msg, "out");

}
