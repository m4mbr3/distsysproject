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
// Implementation of the Remote write protocol
// it is assumed that the messages received are FIFO ordering, ordered by the network module
//
simple RemoteWriteProtocol
{
    parameters:
        //Replica id where the module is configured
        int replicaID = default(-1);
	gates:
	    //Input gate 0 for receiving a message from the invocation manager that has received a message from the network component (from a client or an update from a replica)
        //Input gate 1 for receiving an answer from the data items manager when writting or reading a data item locally
        //Input gate 2 for receiving an answer from the write ahead log of writes (creation and updates) on the data items owned or available in the system
        //Input gate 3 for receiving an answer from the network module (basic network) of a remote write request
        //Input gate 4 for receiving an answer from the network module (t.o multicast) of a remote update
        input in [5];
        //Output gate 0 for sending the processed message to the invocation manager such that it sends back the answer to the client/replica
        //Output gate 1 for sending the processed message to the data items manager for writting (local) or reading (local) the data item 
        //Output gate 2 for sending a write message to the write ahead protocol before executing it locally
        //Output gate 3 for sending the request to the primary server replica of the data item for executing the write (remote) of a data item
        //Output gate 4 for sending the update of a data item that the current replica own
        output out[5];
            
}
