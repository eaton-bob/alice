#include <czmq.h>
#include <stdlib.h>
#include <zyre.h>
#include <malamute.h>

/* !\file   tnh.c
   \details Generates UPS events once a sec, and publishes them to monitor;
            see rfc in top dir
 */

#define STREAM   "METRIC"

static void
s_pub_metric(mlm_client_t * mlm, const char *key, const char *dev_name)
{
    char topic[100], value[100];

    snprintf (topic, 100, "%s.%s", key, dev_name );

    zmsg_t *msg = zmsg_new ();
    zmsg_addstr (msg, dev_name);
    zmsg_addstr (msg, key);
    snprintf (value, 100, "%li", random() % 16 + 10 );
    zmsg_addstr (msg, value);

    mlm_client_send (mlm, topic, &msg);
    printf ("sending %s/%s/%s\n", dev_name, key, value);
}

int main(int argc, char** argv) {
    mlm_client_t *mlm;

    if( argc != 3 ) {
        printf("usage: endpoint name\n");
        return 1;
    }

    const char *NAME = argv[2];
    const char *ENDPOINT = argv[1];

    mlm = mlm_client_new();
    mlm_client_connect ( mlm, ENDPOINT, 5000, NAME );
    mlm_client_set_producer ( mlm, STREAM );
    while( !zsys_interrupted ) {

        s_pub_metric (mlm, "TEMPERATURE", NAME);
        s_pub_metric (mlm, "HUMIDITY", NAME);

        zclock_sleep(5000);
    }
    mlm_client_destroy( &mlm );
}
