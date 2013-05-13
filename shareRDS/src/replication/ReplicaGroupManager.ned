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

package sharerds.replication;

//
// Manager of the replicas available on the system
//
simple ReplicaGroupManager
{
    parameters:
       string ReplicaIDs;
       //The number of replicas managed, the IDs of the replicas
       //should be from 0 to numberReplicas
       int numberReplicas;
       //The replica ID where the Manager is running
       int replicaID;
       int clientID;
    gates:
       //in[0] is the writeAheadProtocol module
       //in[1] is the client reincarnation module
       //in[2] is the Invocation manager gate
       input in[3];
       //out[0] is the writeAheadProtocol module
       //out[1] is the client reincarnation module
       //out[2] is the exit to the compound network module
       output out[3];
}
