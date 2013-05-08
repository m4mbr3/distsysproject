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

import sharerds.compmodules.Replica;
import sharerds.application.AppMsgGenerator;

network ReplicaTest
{
    parameters:
        int nReplicas = default(1);
        int nClients = default(1);

    submodules:
        //The replica
        r0: Replica {
            parameters:
                rID = 0;
                numberReplicas = nReplicas;
                numberClients = nClients;
                @display("p=110,142");
        }
        //put a client
        client: AppMsgGenerator {
            parameters:
                clientID = 0;
                numberReplicas = nReplicas;

                @display("p=39,142");
        }
    connections allowunconnected:
        //connecting the client to the replicas
        client.replicasOut[0] --> r0.inClients[0];
        r0.outClients[0] --> client.replicasIn[0];

}
