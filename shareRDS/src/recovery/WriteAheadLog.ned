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

package sharerds.recovery;

//
// Represents a log used as write ahead log.
//
simple WriteAheadLog
{
    parameters:
        int replicaID= default(-1);
    gates:
        input in; // receiving requests  (write/rollback) from the Replica write protocol for saving a write to a data item
        output out; //sending the answer to the requests coming from the Replica write protocol.
}
