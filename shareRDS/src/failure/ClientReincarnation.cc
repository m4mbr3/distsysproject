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
#include <string>
#include "SystemMsg_m.h"
Define_Module(ClientReincarnation);

void ClientReincarnation::initialize()
{
    // TODO - Generated method body
    clientID = par("clientID");
    ttmsg = new SystemMsg();
    scheduleAt("0.0",ttmsg);
}

void ClientReincarnation::handleMessage(cMessage *msg)
{
    //TODO - Generated method body
    SystemMsg *tmsg = check_and_cast<SystemMsgi*> (msg);
    if (ttmsg == tmsg){
        //I sent a message to myself
        SystemMsg* msg = new SystemMsg();
        //I created a new message with only the isClientReincarnation variable
        //setted.
        msg->setIsClientReincarnation(true);
        send(msg, "out");
    }
    delete tmsg;
}
