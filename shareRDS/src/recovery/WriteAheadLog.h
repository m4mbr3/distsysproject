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

#ifndef __SHARERDS_WRITEAHEADLOG_H_
#define __SHARERDS_WRITEAHEADLOG_H_

#include <omnetpp.h>


/**
 * Implements a write ahead log
 */
class WriteAheadLog : public cSimpleModule
{
  private:
    //the replica id where the log is installed
    int replicaID;
    //Is the map of all the writings executed on each data item
    std::map<std::string, std::vector<int> > dataItemsLog;
    //Is the map of the state of the writings executed on each item
    //1= COMMIT, 0= ROLLBACK
    std::map<std::string, bool> dataItemsState;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
