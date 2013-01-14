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

#include "Txc.h"

Define_Module(Txc);

void Txc::initialize()
{
    //For seeing the variable data in the interface by clicking on the modules of type Txc
       WATCH(counter);
       counter= 10;
       if (par("sendInitialMessage").boolValue())
       {
           EV << "Sending the initial msg\n";
           cMessage *msg = new cMessage("tictocMsg");
           send(msg, "out");
       }
}

void Txc::handleMessage(cMessage *msg)
{
    EV<< "Received message: " << msg->getName() << " sending it again";
    // just send back the message we received
    if(counter >0)
        send(msg, "out");
    counter--;
}
