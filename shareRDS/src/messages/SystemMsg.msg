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

//
// Operation type
//
enum OperationType{
READ=0;
WRITE=1;
COMMIT=2;
ROLLBACK=3;
UPDATE = 4;
ACK = 5;
DELETE = 6;
REINC = 7;
};
//Reply code enum
enum ReplyCodeType{
FAIL=0;
SUCCESS=1;
SUCC_CLIENT=2;
NONE=3;
};
//Message
packet SystemMsg {
    int clientID =-1;		// ID of the client that make the request
    int replicaID=-1;		// ID of the sender
    int replicaOwnerID = -1;	// ID of the owner of dataID
    int lamportClock=-1;
    int replyCode=-1 @enum(ReplyCodeType);
    int operation=-1 @enum(OperationType);
    string dataID;
    int data;
};
