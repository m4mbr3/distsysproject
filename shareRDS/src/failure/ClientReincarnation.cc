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
    // TODO - Generated method body
    clientID = par("clientID");
    ttmsg = new SystemMsg();
    scheduleAt("0.0",ttmsg);
    reinc = new SystemMsg();
    reinc.setIsClientReincarnation(true);
}

void ClientReincarnation::handleMessage(cMessage *msg)
{
    //TODO - Generated method body
    SystemMsg *tmsg = check_and_cast<SystemMsg*> (msg);
    if (tmsg == ttmsg){
        //I sent a message to myself
        SystemMsg* msg = new SystemMsg();
        //I created a new message with only the isClientReincarnation variable
        //setted to say i'm alive again.
        msg->setIsClientReincarnation(true);
        send(msg, "out");
        scheduleAt("500",reinc);
        delete tmsg;
    }
    else if (tmsg == reinc){
        SystemMsg* msg = new SystemMsg();
        msg->setIsClientReincarnation(true);
        if(reinc->getReplyCode()){
            //set i'm dead message to replica group manager
            //prepare i'm alive message for myself
            reinc->setReplyCode(0);
            msg->setReplyCode(1);
        }
        else{
            reinc->setReplyCode(1);
            msg->setReplyCode(0);
        }
        //here i phisically send the i'm alive or i'm dead
        //message to the replica group manager
        //and i schedule the reincarnation message
        //to myself
        send(msg,"out");
        scheduleAt(intuniform(0,500),reinc);
    }
}
