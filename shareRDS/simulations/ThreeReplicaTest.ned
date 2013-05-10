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
// This files allow the simulation of the remote write protocol with THREE replicas and ONE client
//
network ThreeReplicaTest
{
    parameters:
        int nReplicas = default(3);
        int nClients = default(1);

    submodules:
        r0: Replica {
            parameters:
                rID = 0;
                numberReplicas = nReplicas;
                numberClients = nClients;
                @display("p=178,58");
        }
        r1: Replica {
            parameters:
                rID = 1;
                numberReplicas = nReplicas;
                numberClients = nClients;
                @display("p=258,150");
        }
        r2: Replica {
            parameters:
                rID = 2;
                numberReplicas = nReplicas;
                numberClients = nClients;
                @display("p=178,270");
        }
        //put a client
        client: AppMsgGenerator {
            parameters:
                clientID = 0;
                //This client msgs are process by the replica with id 0
                replicaId = 0;
                //Writting and reading a set of data items in the initial connected replica
                operationOrder = "w r";
                dataItemsID = "a a";
                numberReplicas = nReplicas;

                @display("p=82,138");
        }
    connections allowunconnected:

        //Connecting the self loops in each replica
        r0.outReplicas[0] --> r0.inReplicas[0];
        r1.outReplicas[1] --> r1.inReplicas[1];
        r2.outReplicas[2] --> r2.inReplicas[2];

        //Connecting all the replicas to all the ones in the system
        //The channel to replica 1 from replica 0 goes through outReplica[1] of replica 0
        r0.outReplicas[1] --> r1.inReplicas[0];
        //The channel to replica 1 from replica 0 goes through outReplica[2] of replica 0
        r0.outReplicas[2] --> r2.inReplicas[0];
        //The channel to replica 0 from replica 1 goes through outReplica[0] of replica 1
        r1.outReplicas[0] --> r0.inReplicas[1];
        //The channel to replica 0 from replica 1 goes through outReplica[2] of replica 1
        r1.outReplicas[2] --> r2.inReplicas[1];
        //The channel to replica 0 from replica 1 goes through outReplica[0] of replica 1
        r2.outReplicas[0] --> r0.inReplicas[2];
        //The channel to replica 0 from replica 1 goes through outReplica[2] of replica 1
        r2.outReplicas[1] --> r1.inReplicas[2];

        //connecting the client to the replicas
        //Connecting the communication from the client to the replicas
        client.replicasOut[0] --> r0.inClients[0];
        client.replicasOut[1] --> r1.inClients[0];
        client.replicasOut[2] --> r2.inClients[0];
		//Connecting the communication form the replicas to the client
        r0.outClients[0] --> client.replicasIn[0];
        r1.outClients[0] --> client.replicasIn[1];
        r2.outClients[0] --> client.replicasIn[2];
}
