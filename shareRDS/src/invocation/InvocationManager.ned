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
        int replicaID = default(-1);// the replica ID in which the module is configured
    gates:
        input in; //Input gate for receiving messages from the basic network module
        output out; //Output gate for sending the messages to the remote write protocol module
}
