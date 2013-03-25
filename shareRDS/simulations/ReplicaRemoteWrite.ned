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

network ReplicaRemoteWrite
{
    parameters:
        int nReplicas = default(2);
        int nClients = default(1);

    submodules:
        r0: Replica {
            parameters:
                rID =0;
                numberReplicas = nReplicas;
                numberClients = nClients;
                @display("p=94,87");
        }
        r1: Replica {
            parameters:
                rID =1;
                numberReplicas = nReplicas;
                numberClients = nClients;
                @display("p=179,157");
        }
        //put a client
        client: AppMsgGenerator{
        	 parameters:
        	    clientID=1;
                numberReplicas = nReplicas;
        
        };
    connections allowunconnected:

        //Connecting all the replicas to all the ones in the system
        for k=0..r0.numberReplicas-1, if k!=0 {
            r0.outReplicas[k] --> r1.inReplicas[k];
        }
        for k=0..r1.numberReplicas-1, if k!=1 {
            r1.outReplicas[k] --> r0.inReplicas[k];
        }
        //connecting the client to the replicas
		client.replicasOut[0] --> r0.inClients[0];
		client.replicasOut[1] --> r1.inClients[0];
		
		r0.outClients[0] --> client.replicasIn[0];
		r1.outClients[0] --> client.replicasIn[1];
}
