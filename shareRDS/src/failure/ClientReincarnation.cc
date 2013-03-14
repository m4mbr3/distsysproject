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

#include "ClientReincarnation.h"
#include "FailureManager.h"
#include "../messages/SystemMsg_m.h"

#include <string>

Define_Module(ClientReincarnation);

ClientReincarnation::ClientReincarnation(String clientID)
{
    this.clientID = clientID;
    fm = new FailureManager();

}
int ClientReincarnation::getClientID()
{
    return this.clientID;
}
void ClientReincarnation::setClientID(String clientID)
{
    this.clientID = clientID;
}
void ClientReincarnation::initialize()
{
   // TODO - Generated method body

}

void ClientReincarnation::handleMessage(cMessage *msg)
{
    //TODO - Generated method body
    SystemMsg_m *ttmsg = check_and_cast<SystemMsg_m *>(msg)
    if(ttmsg->clientID == this.clientID)
    {
        //I manage the message..
        std::cout << "I received this message" << ttmsg.getData() << "from" << ttmsg.getReplicatID() << std::endl;
    }


}
