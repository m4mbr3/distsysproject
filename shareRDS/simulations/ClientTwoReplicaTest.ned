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

package sharerds.simulations;

import sharerds.compmodules.Client;
import sharerds.compmodules.Replica;


network ClientTwoReplicaTest
{
    parameters:
        int nReplicas = 2;
        int nClients = 1;
        int numberReplicas = 2;
        int cID = 0;
        string ReplicaIDs = "0 1";
        int localClock = 0;
    submodules:
        client: Client {
            clientID = cID;
            ReplicaIDs = ReplicaIDs;
            numberReplicas = numberReplicas;
            localClock = localClock;
            @display("p=388,192");
        }
        replica: Replica {
            rID = 0;
            numberReplicas = nReplicas;
            numberClients = nClients;
            pTimerOffset = 2.0;
            sTimerOffset = 3.0;
            cTimerOffset = 14.0;
            @display("p=736,171");
        }
        replica1: Replica {
            rID = 1;
            numberReplicas = nReplicas;
            numberClients = nClients;
            pTimerOffset = 1.0;
            sTimerOffset = 2.0;
            cTimerOffset = 10.0;
            @display("p=747,325");
        }
    connections allowunconnected:

      //  replica.outReplicas[1] --> replica.inReplicas[1];
       // replica1.outReplicas[1] --> replica1.inReplicas[1];

       // replica.outReplicas[1] --> replica1.inReplicas[1];
       // replica1.outReplicas[1] --> replica.inReplicas[1];

        replica.outClients[0] --> client.inReplicas[0];
        replica1.outClients[0] --> client.inReplicas[1];
        client.outReplicas[0] --> replica.inClients[0];
        client.outReplicas[1] --> replica1.inClients[0];
}
