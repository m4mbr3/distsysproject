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

package sharerds.invocation;

//
// TODO auto-generated module
//
simple InvocationManager
{
    parameters:
        // the id of the current compound module where this module is installed (in the case of the client is the Client ID, and in the case of the replica is the replica ID)
        int entityID = default(-1);
    gates:
        //Input gate 0 for receiving requests from the application (in the case of the client) or from a replica (only applies to replicas)
        //Input gate 1 for receiving the answers from request sent in the compound module to which this module is installed the answer are related
        //to middle steps on the execution of a request (for instance a remote write or an update
        input in[2]; 
        //Output gate 0 for re-sending the requests received to the next of the compound module involved
     	//Output gate 1 for sending the answer to the entity that has invoke something in the current compound module that has installed this module
        output out[2];
}
