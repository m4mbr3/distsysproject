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

package sharerds.compmodules;

import sharerds.invocation.InvocationManager;
import sharerds.replication.*;
import sharerds.recovery.WriteAheadLog;

module Replica
{
    parameters:
    //the replica ID
    int replicaID = default(-1);
    
    gates:
        //For receiving the requests coming from the clients of the simulation
        input inClients[];
        //For receiving the requests coming from the other replicas in the system
        input inReplicas[];
        //For sending answers to the clients in the system
        output outClients[];
        //For sending messages to the other replicas in the system
        output outReplicas[];
    submodules:
        //The invocation manager component
       im: InvocationManager{
       		parameters:
       		    entityID = replicaID;
       };
       //The remote write protocol component
       rwp: RemoteWriteProtocol{
       		parameters:
       		    replicaID = replicaID;
       };
       //The data items manager
       dim: DataItemsManager{
           parameters:
       		    replicaID = replicaID;
       };
       //The write ahead log
       wal: WriteAheadLog{
       		parameters:
       		    replicaID = replicaID;
       };
      connections:
        //NW and IM     
        //RWP and IM
        im.out[0] --> rwp.in[0];
        rwp.out[0] --> im.in[1];
        //RWP and DIM  
       rwp.out[1] --> dim.in;
       dim.out --> rwp.in[1];
       //RWP and WAL
       rwp.out[2] --> wal.in;
       wal.out --> rwp.in[2];
       //RWP and NW
       //NW and LCLOCK
       //REPLICA AND NW
       
}
