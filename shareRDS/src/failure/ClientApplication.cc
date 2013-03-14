#include "ClientApplication.h"
#include "../messages/SystemMsg_m.h"
void ClientApplication::initialize()
{

}

void ClientApplication::handleMessage(cMessage *msg)
{
    SystemMsg *ttmsg = check_and_cast<SystemMsg*>(msg);

}

