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
//
// TODO auto-generated type
//
network TwoReplicaTest2
{
    parameters:
        int nReplicas = default(2);
        int nClients = default(2);

    submodules:
        r0: Replica {
            parameters:
                rID = 0;
                numberReplicas = nReplicas;
                numberClients = nClients;
                pTimerOffset = 2.0;
                sTimerOffset = 3.0;
                cTimerOffset = 20.0;
                @display("p=140,58");
        }
        r1: Replica {
            parameters:
                rID = 1;
                numberReplicas = nReplicas;
                numberClients = nClients;
                pTimerOffset = 2.0;
                sTimerOffset = 3.0;
                cTimerOffset = 14.0;
                @display("p=140,195");
        }
        //Clients
        c0: AppMsgGenerator {
            parameters:
                clientID = 0;
                //This client msgs are process by the replica with id 0
                replicaId = 0;
                //Writting and reading a set of data items in the initial connected replica
                operationOrder = "w";
                dataItemsID = "a";
                numberReplicas = nReplicas;
				timerOffset = 1.0;
                @display("p=43,107");
        }
                //put a client
        c1: AppMsgGenerator {
            parameters:
                clientID = 1;
                //This client msgs are process by the replica with id 0
                replicaId = 1;
                //Writting and reading a set of data items in the initial connected replica
                operationOrder = "w";
                dataItemsID = "a";
                numberReplicas = nReplicas;
				timerOffset = 15.0;
                @display("p=249,120");
        }
    connections allowunconnected:

        //Connecting the self loops in each replica
        r0.outReplicas[0] --> r0.inReplicas[0];
        r1.outReplicas[1] --> r1.inReplicas[1];

        //Connecting all the replicas to all the ones in the system
        //The channel to replica 1 from replica 0 goes through outReplica[1] of replica 0
        r0.outReplicas[1] --> r1.inReplicas[0];
        //The channel to replica 0 from replica 1 goes through outReplica[0] of replica 1
        r1.outReplicas[0] --> r0.inReplicas[1];
        //connecting the client to the replicas
        //Connecting the communication from the client to the replicas
        //Connecting c0
        c0.replicasOut[0] --> r0.inClients[0];
        c0.replicasOut[1] --> r1.inClients[0];
        //Connecting c1
        c1.replicasOut[0] --> r0.inClients[1];
        c1.replicasOut[1] --> r1.inClients[1];
		//Connecting the communication form the replicas to the client
		//to C0
        r0.outClients[0] --> c0.replicasIn[0];
        r1.outClients[0] --> c0.replicasIn[1];
        //To C1
        r0.outClients[1] --> c1.replicasIn[0];
        r1.outClients[1] --> c1.replicasIn[1];
}
