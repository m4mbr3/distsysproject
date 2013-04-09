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
        input inRemoteRequests[2];		// from RemoteWriteProtocol
        							// [0] for remoteWrite
    								// [1] for remoteUpdate
        input inClients[];			// from clients
		input inReplicas[];			// from other replica
		input inAnswer;				// from InvocationManager
		//input inLamportClk;
		
		output outAnswer[2] ;		// to RemoteWriteProtocol
									// [0] for remoteWrite
									// [1] for remoteUpdate
		output outClients[];
		output outReplicas[];
		output outRequest;       	// to InvocationManager
        //output outLamportNew;		// to LamportClock for setting new lamport clock value
        //output outLamportUpt;		// to Lamport Clock for update lamport clock value only
}