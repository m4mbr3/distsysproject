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

package sharerds.simulations.ClientSimulation;

import sharerds.compmodules.Client;
import sharerds.compmodules.Replica;
import sharerds.invocation.InvocationManager;
import sharerds.replication.*;
import sharerds.recovery.WriteAheadLog;
import sharerds.networks.ReplicaNetwork;


//
// TODO documentation
//
network Network
{
    parameters:
        int nReplicas = 1;
        int nClients = 1;
        int numberReplicas = 1;
        int cID = 0;
        string ReplicaIDs = "0";
        int localClock = 0;

    submodules:

        client: Client {
            clientID = cID;
            ReplicaIDs = ReplicaIDs;
            numberReplicas = numberReplicas;
            localClock = localClock;
            willfail = true;
            is_large = false;
            @display("p=388,192;i=device/pc");
        }

        replica: Replica {
            rID = 0;
            numberReplicas = nReplicas;
            numberClients = nClients;
            pTimerOffset = 2.0;
            sTimerOffset = 3.0;
            cTimerOffset = 14.0;
            @display("p=194,70");
        }
    connections allowunconnected:
        replica.outClients[0] --> client.inReplicas[0];
        client.outReplicas[0] --> replica.inClients[0];

}

