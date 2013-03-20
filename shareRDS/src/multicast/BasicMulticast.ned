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

package sharerds.multicast;

//enum OperationType {
//	WRITE = 1;
//	UPDATE = 0;
//};
//const WRITE_OP = 1;
//const UPDATE_OP = 0;

simple BasicMulticast
{
    parameters:
        int replicaID;				// replicaID for InvocationManager
        int operation = default(-1);		// operation in request
	        // operation = 0 	// UPDATE
	        // operation = 1 	// WRITE
    gates:
        input inRmtWrtProtocol;				// input gate for receiving request from RemoteWriteProtocol
        output outInvocMan;			// output gate for sending request to InvocationManager
        input inLamportClk;
        output outLamportClk;
        
        input inReplica[5];
        
        
    //connections:
        //TODO: define all connections below if needed
    
}
