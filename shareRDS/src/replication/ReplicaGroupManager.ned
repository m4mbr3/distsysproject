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
       //The number of replicas managed, the IDs of the replicas
       //should be from 0 to numberReplicas
       int numberReplicas= default(1);
       //The replica ID where the Manager is running 
       int replicaID = default(-1); 
    gates:
        input in;
        output out;
}
