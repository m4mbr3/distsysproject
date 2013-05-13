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
        int cID = default(-1);
        int numberReplicas = default(1);

        @display("bgb=1019,568");
    gates:
        input inReplicas[numberReplicas];
        input outReplicas[numberReplicas];

    types:
        channel Unnamed
        {
        }

    submodules:
        application: Application {
            parameters:

                @display("p=175,28");
        }
        invocationManager: InvocationManager {
            parameters:

                @display("p=221,120");
        }
        replicaGroupManager: ReplicaGroupManager {
            parameters:

                @display("p=221,199");
        }
        writeAhead: WriteAheadLog {
            parameters:

                @display("p=126,266");
        }
        clientReincarnation: ClientReincarnation {
            parameters:
                @display("p=297,266");
        }
        clientNetwork: ClientNetwork {
            parameters:
                @display("p=299,474");
            gates:
                inReplicas[numberReplicas];
                outReplicas[numberReplicas];

        }

        basicNetwork: BasicNetwork {
            @display("p=193,329;is=l");
        }
        lamport: LamportClockClient {
            @display("p=473,399");
        }

    connections:
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
        lamport.out[1] --> basicNetwork.in[1];
        basicNetwork.out[1] --> lamport.in[1];
}

