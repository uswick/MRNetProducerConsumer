/****************************************************************************
 * Copyright � 2003-2012 Dorian C. Arnold, Philip C. Roth, Barton P. Miller *
 *                  Detailed MRNet usage rights in "LICENSE" file.          *
 ****************************************************************************/

#include "mrnet/MRNet.h"
#include "MRNetProdCon.h"


using namespace MRN;

int main(int argc, char **argv)
{
    Stream * stream = NULL;
    PacketPtr p;
    int rc, tag=0, num_iters=0;
    int send_val = 10 ;
    int recv_val ;

    int sendAr[TOTAL_PACKET_SIZE];

    Network * net = Network::CreateNetworkBE( argc, argv );

    do {
        
        rc = net->recv(&tag, p, &stream);
        if( rc == -1 ) {
            fprintf( stderr, "BE: Network::recv() failure\n" );
            break;
        }
        else if( rc == 0 ) {
            // a stream was closed
            continue;
        }
         
        switch(tag) {

        case PROT_CONCAT:
            p->unpack( "%d %d", &recv_val, &num_iters );

            printf("Init BE pid : %d : values :", getpid());

                // Send integer arrays as waves - simulate buffer waves
            for( int i=0; i<num_iters; i++ ) {
                //init array for each wave
                initBuffer(sendAr);

                fprintf( stdout, "BE: Sending wave %u ...\n", i );
//                if( stream->send(tag, "%d", send_val + i) == -1 ) {
                if( stream->send(tag, "%ad", sendAr, TOTAL_PACKET_SIZE) == -1 ) {
                    fprintf( stderr, "BE: stream::send(%%d) failure in PROT_CONCAT\n" );
                    tag = PROT_EXIT;
                    break;
                }
                if( stream->flush() == -1 ) {
                    fprintf( stderr, "BE: stream::flush() failure in PROT_CONCAT\n" );
                    break;
                }
                fflush(stdout);
                sleep(2); // stagger sends
            }
            break;

        case PROT_EXIT:
            if( stream->send(tag, "%d", 0) == -1 ) {
                fprintf( stderr, "BE: stream::send(%%s) failure in PROT_EXIT\n" );
                break;
            }
            if( stream->flush( ) == -1 ) {
                fprintf( stderr, "BE: stream::flush() failure in PROT_EXIT\n" );
            }
            break;

        default:
            fprintf( stderr, "BE: Unknown Protocol: %d\n", tag );
            tag = PROT_EXIT;
            break;
        }

        fflush(stderr);

    } while( tag != PROT_EXIT );

    if( stream != NULL ) {
        while( ! stream->is_Closed() )
            sleep(1);

        delete stream;
    }

    // FE delete of the net will cause us to exit, wait for it
    net->waitfor_ShutDown();
    delete net;

    return 0;
}

void initBuffer(int* sendAr) {
    srand(time(NULL) + getpid());
    for(int i = 0; i < TOTAL_PACKET_SIZE ; i++){
        sendAr[i] = (rand() % 100) + 1;
        printf(" %d:", sendAr[i]);
        if(i == (TOTAL_PACKET_SIZE - 1)) printf("\n");
    }

}

