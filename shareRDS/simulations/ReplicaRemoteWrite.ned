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

import sharerds.compmodules.*;

network ReplicaRemoteWrite
{
    parameters:
        int nReplicas = default(2);
        int nClients = default(1);

    submodules:
        r1: Replica {
            parameters:
                numberReplicas = nReplicas;
                numberClients = nClients;
                @display("p=94,87");
        }
        r2: Replica {
            parameters:
                numberReplicas = nReplicas;
                numberClients = nClients;
                @display("p=179,157");
        }
        //put a client
    connections allowunconnected:

        //Connecting all the replicas to all the ones in the system
        for k=0..r1.numberReplicas-1 {
            r1.outReplicas[k] --> r2.inReplicas[k];
        }
        for k=0..r2.numberReplicas-1 {
            r2.outReplicas[k] --> r1.inReplicas[k];
        }
        //connecting the client to the replicas

}
