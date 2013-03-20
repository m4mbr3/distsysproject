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

package sharerds.networks;

//
// TODO auto-generated module
//
simple ReplicaNetwork
{
    parameters:
        int replicaID = default(-1);		// ID of the owner of this network
    gates:
        input inRequests[2];		//from RemoteWriteProtocol
        input inClients[];
		input inReplicas[];
		input inAnswer;			// from InvocationManager
		input inLamportClk;
		
		output outRequests[2] ;	// to RemoteWriteProtocol
		output outClients[];
		output outReplicas[];
		output outAnswer;       	// to InvocationManager
        output outLamportNew;		// to LamportClock for setting new lamport clock value
        output outLamportUpt;		// to Lamport Clock for update lamport clock value only
}
