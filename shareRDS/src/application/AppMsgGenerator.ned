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

package sharerds.application;

//
// Generates messages of read or write into the system from a client of the system
//
simple AppMsgGenerator
{
    parameters:
        //The client id, must be unique in the system
        int clientID = default(-1);
        //The replica id, must be one of the ids of existing replicas in the simulation
        //This replica id is the one to which this msg generator will send the msgs
        int replicaId = default(-1);
        //The number of replicas
        int numberReplicas = default(-1);
        //Defines the type of operations that the client should execute during the simulation, SEPARATED BY WHITE SPACE
        //w = write operation
        //r = read operation
        string operationOrder;
        //Defines the data items id, related to the operations define in operation order, SEPARATED BY WHITE SPACE
        string dataItemsID;
        //the time for the timer related to the sending of requests
        double timerOffset = default(1.0);
        @display("i=device/laptop");
    gates:
        input replicasIn[numberReplicas];
        output replicasOut[numberReplicas];
}
