#include <malamute.h>

int main(int argc, char **argv)
{
    if(argc != 2) {
        fprintf(stderr, "Usage: %s address\n", argv[0]);
        exit(1);
    }

    char *addr = argv[1];

    mlm_client_t *agent = mlm_client_new ();
    // mlm_client_connect () returns -1 on failure
    if ( mlm_client_connect (agent, addr, 1000, "UI") == -1 ) {
        zsys_error ("Cannot connect to the endpoint %s", addr);
        mlm_client_destroy (&agent);
        return EXIT_FAILURE;
    }
    zmsg_t *getmsg = zmsg_new();
    zmsg_addstr(getmsg, "GET");
    zmsg_addstr(getmsg, "temperature");
    // mlm_client_sendfor () returns zero on success
    if (mlm_client_sendfor (agent, "stats", "temperature", NULL, 0, &getmsg) !=0 ) {
        zsys_error ("Cannot send the message");
        mlm_client_destroy (&agent);
        return 1;
    }
    getmsg = zmsg_new();
    zmsg_addstr(getmsg, "GET");
    zmsg_addstr(getmsg, "ups.state");
    if (mlm_client_sendfor (agent, "stats", "ups.state", NULL, 0, &getmsg) != 0) {
        zsys_error ("Cannot send the message");
        mlm_client_destroy (&agent);
        return 1;
    }
    getmsg = zmsg_new();
    zmsg_addstr(getmsg, "GET");
    zmsg_addstr(getmsg, "ups.power");
    if (mlm_client_sendfor (agent, "stats", "ups.power", NULL, 0, &getmsg) != 0) {
        zsys_error ("Cannot send the message");
        mlm_client_destroy (&agent);
        return 1;
    }

    // need to read replies, waiying for 3 of them
    int i = 0;
    while ( !zsys_interrupted && i < 3 )
    {
        zsys_info ("waiting for %d reply message", i);
        zmsg_t *msg = mlm_client_recv (agent);
        if ( msg == NULL )
            continue;
        if ( streq (mlm_client_command (agent), "MAILBOX DELIVER") )
        {
            if ( ( strcmp(mlm_client_subject(agent), "temperature") == 0 ) ||
                 ( strcmp(mlm_client_subject(agent), "ups.power") == 0 ) ||
                 ( strcmp(mlm_client_subject(agent), "ups.temperature") == 0 ) )
            {
                char *key = zmsg_popstr (msg);
                char *result = zmsg_popstr (msg);
                zsys_info ("GOT: %s = %s", key , result);
                i++;
            }
        }
        zmsg_destroy (&msg);
    }
    mlm_client_destroy (&agent);
    return 0;
}
