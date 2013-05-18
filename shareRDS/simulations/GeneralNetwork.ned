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

network GeneralNetwork 
{
    parameters:
        int nReplicas = default(2);
        int nClients = default(2);

    submodules:
        // Generate clients
    	client[nClients] : AppMsgGenerator {
    	    clientID = index;
    	    // set replicaId to connect this client to. based on fmod
    	    replicaId = fmod(index,nReplicas) ;
    		numberReplicas = nReplicas;
    		operationOrder = default("w r");
            dataItemsID = default("a a");
            timerOffset = default(3.0);
    	}
        
        // Generate replicas
    	replica[nReplicas] : Replica {
        	rID = index;
            numberReplicas = nReplicas;
            numberClients = nClients;
            pTimerOffset = default(1.0);
            sTimerOffset = default(2.0);
            cTimerOffset = default(10.0);
    	}
        
        
    connections allowunconnected:
        // Connect replicas and clients
        for clientNo = 0..nClients - 1, for replicaNo = 0..nReplicas - 1  {
    		client[clientNo].replicasOut[replicaNo] --> replica[replicaNo].inClients[clientNo];
    		replica[replicaNo].outClients[clientNo] --> client[clientNo].replicasIn[replicaNo];
        }
        // Connect replicas and replicas
        for srcReplica = 0..nReplicas-1, for destReplica = 0..nReplicas-1{
        	replica[srcReplica].outReplicas[destReplica] --> replica[destReplica].inReplicas[srcReplica];
        }
        
        
}