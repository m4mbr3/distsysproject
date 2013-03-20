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
#include "SystemMsg_m.h"
#include <string>

Define_Module(ClientReincarnation);

/*
ClientReincarnation::ClientReincarnation(std::string clientID)
{
    this.clientID = clientID;
    fm = new FailureManager();

}
*/
int ClientReincarnation::getClientID()
{
    return clientID;
}
void ClientReincarnation::setClientID(int clientID)
{
    this->clientID = clientID;
}
void ClientReincarnation::initialize()
{
    //myOwnData->
}

void ClientReincarnation::handleMessage(cMessage *msg)
{
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);
    if(ttmsg->getClientID() == clientID)
    {
        //I manage the message..if and only if i'm not failed
        if(!fm->isFailed())
           std::cout << "I received this message" << ttmsg->getData() << "from" << ttmsg->getReplicaID() << std::endl;

    }


}
