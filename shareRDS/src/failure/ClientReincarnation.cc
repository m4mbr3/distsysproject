// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "ClientReincarnation.h"


Define_Module(ClientReincarnation);

void ClientReincarnation::initialize()
{
    clientID = par("clientID");
    reinc = new SystemMsg();
    reinc->setReplyCode(0);
    //if (willfail)
    scheduleAt(simTime()+ intuniform(0,400),reinc);
}

void ClientReincarnation::handleMessage(cMessage *msg)
{
    SystemMsg *reinc = check_and_cast<SystemMsg*> (msg);
    if (reinc->isSelfMessage() ){
        SystemMsg* msg_new = new SystemMsg();
        if(reinc->getReplyCode()){
            //set i'm dead message to replica group manager
            //prepare i'm alive message for myself
            reinc->setReplyCode(0);
            msg_new->setReplyCode(1);
        }
        else{
            reinc->setReplyCode(1);
            msg_new->setReplyCode(0);
        }
        //here i phisically send the i'm alive or i'm dead
        //message to the replica group manager
        //and i schedule the reincarnation message
        //to myself
        send(msg_new,"out");
        scheduleAt(intuniform(0,500)+simTime(),reinc);
    }
}
