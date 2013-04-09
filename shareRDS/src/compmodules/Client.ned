package sharerds.compmodules;

import sharerds.application.Application;
import sharerds.invocation.InvocationManager;
import sharerds.replication.ReplicaGroupManager;
import sharerds.recovery.WriteAheadProtocol;
import sharerds.failure.ClientReincarnation;
import sharerds.networks.ClientNetwork;
import sharerds.networks.BasicNetwork;


module Client
{
    parameters:
         int cID = default(-1);
         int numberReplicas = default(1);

    gates:
        input inReplicas[numberReplicas];
        input outReplicas[numberReplicas];

    submodules:
        ap: Application{
            parameters:

            }
        im: InvocationManager{
            parameters:

        }
        rgm: ReplicaGroupManager{
            parameters:

        }
        whp: WriteAheadProtocol{
            parameters:

        }
        cr : ClientReincarnation{
            parameters:
        }
        cw: ClientNetwork{
            parameters:
            gates:
                inReplicas[numberReplicas];
                outReplicas[numberReplicas];

        }

    connections:
        //Application module and InvocationManager
        ap.out --> im.in[0];
        im.out[1] --> ap.in;
        im.out[0] -->rgm.in[2];
        rgm.out[0] --> whp.in;
        whp.out --> rgm.in[0];
        rgm.out[1] --> cr.in;
        cr.out --> rgm.in[1];
        rgm.out[2] -->

        //TODO
}

