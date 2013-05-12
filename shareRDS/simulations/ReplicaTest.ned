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
                replicaId = 0;
                operationOrder ="w w r r";
                dataItemsID = "a b b a";
                numberReplicas = nReplicas;
                @display("p=39,142");
        }
    connections allowunconnected:
        //connecting the client to the replicas
        client.replicasOut[0] --> r0.inClients[0];
        r0.outClients[0] --> client.replicasIn[0];
        //IMPORTANT
        //Connecting the outs to other replicas 
        //As R0 is the only one replica we must connect the R0.outReplica[0] to its own inReplica[0]
        r0.outReplicas[0] --> r0.inReplicas[0];

}
