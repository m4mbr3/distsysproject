package sharerds.compmodules;

import sharerds.application.Application;
import sharerds.invocation.InvocationManager;
import sharerds.replication.ReplicaGroupManager;
import sharerds.failure.ClientReincarnation;
import sharerds.networks.ClientNetwork;
import sharerds.networks.BasicNetwork;
import sharerds.recovery.WriteAheadLog;


module Client
{
    parameters:
        int cID = default(-1);
        int numberReplicas = default(1);

        @display("bgb=461,433");
    gates:
        input inReplicas[numberReplicas];
        input outReplicas[numberReplicas];

    types:
        channel Unnamed
        {
        }

    submodules:
        ap: Application {
            parameters:

                @display("p=221,30");
        }
        im: InvocationManager {
            parameters:

                @display("p=221,120");
        }
        rgm: ReplicaGroupManager {
            parameters:

                @display("p=221,199");
        }
        whp: WriteAheadLog {
            parameters:

                @display("p=126,266");
        }
        cr: ClientReincarnation {
            parameters:
                @display("p=297,266");
        }
        cw: ClientNetwork {
            parameters:
            gates:
                inReplicas[numberReplicas];
                outReplicas[numberReplicas];

        }

        basicNetwork: BasicNetwork {
            @display("p=211,343;is=l");
        }
        clientNetwork: ClientNetwork {
            @display("p=396,317");
        }
    connections:
        //Application module and InvocationManager
        ap.out --> im.in[0];
        im.out[1] --> ap.in;
        im.out[0] --> rgm.in[2];
        rgm.out[0] --> whp.in;
        whp.out --> rgm.in[0];
        rgm.out[1] --> cr.in;
        cr.out --> rgm.in[1];
        //rgm.out[2] -->

        //TODO
        rgm.out[0] --> basicNetwork.in[0];
        basicNetwork.out[0] --> rgm.in[0];
        basicNetwork.out[0] --> clientNetwork.inReplicas++;
        clientNetwork.outReplicas++ --> basicNetwork.in[0];
        im.out[0] --> clientNetwork.inReplicas++;
        clientNetwork.outReplicas++ --> im.in[0];
}

