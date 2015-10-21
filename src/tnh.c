#include <czmq.h>
#include <stdlib.h>
#include <zyre.h>
#include <malamute.h>

/* !\file   tnh.c
   \details Generates UPS events once a sec, and publishes them to monitor;
            see rfc in top dir
 */

#define STREAM   "METRIC"

int main(int argc, char** argv) {    
    mlm_client_t *mlm;
    char topic[100], message[100];

    if( argc != 3 ) {
        printf("usage: endpoint name\n");
        return 1;
    }

    const char *NAME = argv[2];
    const char *ENDPOINT = argv[1];

    mlm = mlm_client_new();
    mlm_client_connect ( mlm, ENDPOINT, 5000, NAME );
    mlm_client_set_producer ( mlm, "METRICS" );
    while( !zsys_interrupted ) {
        snprintf(message, 100, "%s/TEMPERATURE/%i", NAME, random() % 16 + 10 );
        snprintf(topic, 100, "temperature.%s", NAME );
        mlm_client_sendx( mlm, topic, message, NULL); 
        printf("sending %s\n", message);
        
        snprintf(message, 100, "%s/HUMIDITY/%i", NAME, random() % 30 + 50 );
        snprintf(topic, 100, "humidity.%s", NAME );
        mlm_client_sendx( mlm, topic, message, NULL); 
        printf("sending %s\n", message);

        zclock_sleep(5000);
    }
    mlm_client_destroy( &mlm );
}
