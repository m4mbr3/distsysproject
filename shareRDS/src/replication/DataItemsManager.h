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

#ifndef __SHARERDS_DATAITEMSMANAGER_H_
#define __SHARERDS_DATAITEMSMANAGER_H_

#include <omnetpp.h>


/**
 * CONSTANTS
 */
#define NO_DATA -1

/**
 * Data items manager simple module.
 * This module is in charge in managing the data items (id versus the related data) on the replica with the id saved in replicaID
 */
class DataItemsManager : public cSimpleModule
{
  private:
  //This map contains the data items with it respective id and data
  std::map<std::string, int> dataItems;
  //The replica id where the dataitem manager belongs to
  int replicaID;

  public:
  DataItemsManager();
  virtual ~DataItemsManager();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
