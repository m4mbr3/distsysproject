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
import sharerds.networks.ReplicaNetwork;

module Replica
{
    parameters:
        //the replica ID
        int rID = default(-1);
        //the total number of replicas available
        int numberReplicas = default(1);
        //the total number of clients in the system
        int numberClients = default(1);
        //Timers parameters
        double pTimerOffset = default(2.0);
        double sTimerOffset = default(3.0);
        double cTimerOffset = default(14.0);
		//Display parameters
        @display("i=block/buffer2;bgb=321,354");

    gates:
        //For receiving the requests coming from the clients of the simulation
        input inClients[numberClients];
        //For receiving the requests coming from the other replicas in the system
        input inReplicas[numberReplicas];
        //For sending answers to the clients in the system
        output outClients[numberClients];
        //For sending messages to the other replicas in the system
        output outReplicas[numberReplicas];
    submodules:
        //The invocation manager component
        im: InvocationManager {
            parameters:
                entityID = rID;
                @display("p=138,45;i=block/downarrow");
        }
        //The remote write protocol component
        rwp: RemoteWriteProtocol {
            parameters:
                replicaID = rID;
                @display("p=86,108;i=block/cogwheel");
        }
        //The data items manager
        dim: DataItemsManager {
            parameters:
                replicaID = rID;
                @display("p=166,108;i=block/buffer2");
        }
        //The write ahead log
        wal: WriteAheadLog {
            parameters:
                replicaID = rID;
                @display("p=26,117;i=block/table");
        }
        //the network module
        nw: ReplicaNetwork {
            parameters:
                replicaID = rID;
                processingTimerOffset = pTimerOffset;
                sendingTimerOffset = sTimerOffset;
                checkAckTimerOffset = cTimerOffset;
                @display("p=99,216;i=block/broadcast");
            gates:
                inClients[numberClients];
                inReplicas[numberReplicas];
                outClients[numberClients];
                outReplicas[numberReplicas];
        }
    connections:
        //NW and IM
        nw.outRequest --> im.in[0];
        im.out[1] --> nw.inAnswer;
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
        //remote write
        rwp.out[3] --> nw.inRemoteRequests[0];
        nw.outAnswer[0] --> rwp.in[3];
        //remote update
        rwp.out[4] --> nw.inRemoteRequests[1];
        nw.outAnswer[1] --> rwp.in[4];
        //REPLICA and NW
        //inClients
        for i=0..sizeof(inClients)-1 {
            inClients[i] --> nw.inClients[i];
        }
        //inReplicas
        for i=0..sizeof(inReplicas)-1 {
            inReplicas[i] --> nw.inReplicas[i];
        }
        //outClients
        for i=0..sizeof(outClients)-1 {
            nw.outClients[i] --> outClients[i];
        }
        //outReplicas
        for i=0..sizeof(outReplicas)-1 {
            nw.outReplicas[i] --> outReplicas[i];
        }
}
