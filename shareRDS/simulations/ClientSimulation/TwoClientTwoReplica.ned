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
// https://guest.polimi.it/login.html?redirect=corsi.dei.polimi.it/distsys

package sharerds.simulations.ClientSimulation;
import sharerds.compmodules.Client;
import sharerds.compmodules.Replica;


//
// TODO documentation
//
network Network1
{
    parameters:
        int nReplicas = 3;
        int nClients = 2;
        int numberReplicas = 2;
        int cID = 0;
        int cID1 = 1;
        string ReplicaIDs = "0 1";
        int localClock = 0;
        bool is_large = false;
        bool willfail = true;
        @display("bgb=376,313;bgl=9");
    types:
        channel Unnamed
        {
        }

    submodules:
        client: Client {
            clientID = cID;
            ReplicaIDs = ReplicaIDs;
            numberReplicas = numberReplicas;
            localClock = localClock;
            is_large = is_large;
            willfail = willfail;
            @display("p=108,215");
        }
        client1: Client {
            clientID = cID1;
            ReplicaIDs = ReplicaIDs;
            numberReplicas = numberReplicas;
            localClock = localClock;
            willfail = willfail;
            @display("p=256,50");
        }
        replica: Replica {
            rID = 0;
            numberReplicas = nReplicas;
            numberClients = nClients;
            pTimerOffset = 2.0;
            sTimerOffset = 3.0;
            cTimerOffset = 14.0;
            @display("p=83,50");

        }
        replica1: Replica {
            rID = 1;
            numberReplicas = nReplicas;
            numberClients = nClients;
            pTimerOffset = 2.0;
            sTimerOffset = 3.0;
            cTimerOffset = 14.0;
            @display("p=295,204");

        }
    connections allowunconnected:
        //replica to client
        replica.outClients[0] --> client.inReplicas[0];
        client.outReplicas[0] --> replica.inClients[0];
        //replica to client1
        replica.outClients[1] --> client1.inReplicas[0];
        client1.outReplicas[0] --> replica.inClients[1];
        //replica1 to client
        replica1.outClients[0] --> client.inReplicas[1];
        client.outReplicas[1] --> replica1.inClients[0];
        //replica1 to client1
        replica1.outClients[1] --> client1.inReplicas[1];
        client1.outReplicas[1] --> replica1.inClients[1];
        replica.outReplicas[0] --> replica1.inReplicas[0];
        replica1.outReplicas[0] --> replica.inReplicas[0];
        replica1.outReplicas[2] --> replica1.inReplicas[2];
        replica.outReplicas[2] --> replica.inReplicas[2];
}

