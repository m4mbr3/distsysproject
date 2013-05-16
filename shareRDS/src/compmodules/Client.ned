package sharerds.compmodules;

import sharerds.application.Application;
import sharerds.invocation.InvocationManager;
import sharerds.replication.ReplicaGroupManager;
import sharerds.failure.ClientReincarnation;
import sharerds.networks.ClientNetwork;
import sharerds.networks.BasicNetwork;
import sharerds.recovery.WriteAheadLog;
import sharerds.synchronization.LamportClockClient;


module Client
{
    parameters:
        int clientID = default(-1);
        int numberReplicas = default(2);
        string ReplicaIDs;
        int replicaID = default(0);
        int localClock = default(0);
        bool is_large = default (true);
        bool willfail = default (true);
        @display("bgb=1019,568;bgl=2");
    gates:
        input inReplicas[numberReplicas+2];
        output outReplicas[numberReplicas+2];

    types:
        channel Unnamed
        {
        }

    submodules:
        application: Application {
            parameters:
                clientID = clientID;
                is_large = is_large;
                @display("p=175,28");
        }
        invocationManager: InvocationManager {
            parameters:
                entityID = clientID;
                @display("p=221,120");
        }
        replicaGroupManager: ReplicaGroupManager {
            parameters:
                clientID = clientID;
                ReplicaIDs = ReplicaIDs;
                replicaID = replicaID;
                numberReplicas = numberReplicas+2;
                willfail = willfail;
                @display("p=221,199");
        }
        writeAhead: WriteAheadLog {
            parameters:
                replicaID = clientID;
                @display("p=126,266");
        }
        clientReincarnation: ClientReincarnation {
            parameters:
                clientID = clientID;
                willfail = willfail;
                @display("p=297,266");
        }
        clientNetwork: ClientNetwork {
            parameters:
                @display("p=299,474");
            gates:
                inReplicas[numberReplicas+2];
                outReplicas[numberReplicas+2];

        }

        basicNetwork: BasicNetwork {
            @display("p=193,329;is=l");
        }
        lamport: LamportClockClient {
            localClock = localClock;
            @display("p=473,399");
        }

    connections allowunconnected:
        //Application module and InvocationManager
        application.out --> invocationManager.in[0];
        invocationManager.out[1] --> application.in;
        invocationManager.out[0] --> replicaGroupManager.in[2];
        replicaGroupManager.out[0] --> writeAhead.in;
        writeAhead.out --> replicaGroupManager.in[0];
        replicaGroupManager.out[1] --> clientReincarnation.in;
        clientReincarnation.out --> replicaGroupManager.in[1];
        replicaGroupManager.out[2] --> basicNetwork.in[0];
        basicNetwork.out[0] --> invocationManager.in[1];
        clientNetwork.outReplicas[1] --> basicNetwork.in[1];
        basicNetwork.out[1] --> clientNetwork.inReplicas[1];
        clientNetwork.outReplicas[0] --> lamport.in[0];
        lamport.out[0] --> clientNetwork.inReplicas[0];
        lamport.out[1] --> basicNetwork.in[2];
        basicNetwork.out[2] --> lamport.in[1];
        for i=2..sizeof(inReplicas)-1 {
            inReplicas[i-2] --> clientNetwork.inReplicas[i];
        }
        for i=2..sizeof(outReplicas)-1 {
            clientNetwork.outReplicas[i] --> outReplicas[i-2];
        }
}

