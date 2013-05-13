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

import sharerds.application.AppMsgGenerator;
import sharerds.compmodules.Replica;


network TwoClientsOneRep
{
    parameters:
        int nReplicas = default(1);
        int nClients = default(2);
        
    submodules:
        client0: AppMsgGenerator {
            parameters:
                clientID = 0;
                //This client msgs are process by the replica with id 0
                replicaId = 0;
                //Writting and reading a set of data items in the initial connected replica
                operationOrder ="w r";
                dataItemsID = "a b";
                numberReplicas = nReplicas;
	            @display("p=48,38");
        }
        client1: AppMsgGenerator {
            parameters:
                clientID = 1;
                //This client msgs are process by the replica with id 0
                replicaId = 0;
                //Writting and reading a set of data items in the initial connected replica
                operationOrder ="w r";
                dataItemsID = "b a";
                numberReplicas = nReplicas;
           	 	@display("p=48,162");
        }
        replica: Replica {
            parameters:
                rID = 0;
                numberReplicas = nReplicas;
                numberClients = nClients;
                pTimerOffset = 1.0;
                sTimerOffset = 2.0;
                cTimerOffset = 10.0;
                @display("p=175,107");
        }
    connections allowunconnected:
        // Connect the self-loop for the replica
        replica.outReplicas[0] --> replica.inReplicas[0];
        // Connect the replica and the clients
        //		from clients to replica
        client0.replicasOut[0] --> replica.inClients[0];
        client1.replicasOut[0] --> replica.inClients[1];
        //		from replica to clients
        replica.outClients[0] --> client0.replicasIn[0];
        replica.outClients[1] --> client1.replicasIn[0];
}
