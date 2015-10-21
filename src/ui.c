#include <malamute.h>

zmsg_t *computation_message (const char *key, const char *dev_name) {
    assert (key);
    zmsg_t *cmp_msg = zmsg_new ();
    if (!cmp_msg)
        return NULL;
    zmsg_addstr(cmp_msg, "GET");
    zmsg_addstrf(cmp_msg, "%s.%s", key, dev_name);
    return cmp_msg;
}

int main (int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s address ups_name\n", argv[0]);
        exit(1);
    }

    char *addr = argv[1];
    char *name = argv[2];

    mlm_client_t *agent = mlm_client_new ();
    // mlm_client_connect () returns -1 on failure
    if ( mlm_client_connect (agent, addr, 1000, "UI") == -1 ) {
        zsys_error ("Cannot connect to the endpoint %s", addr);
        mlm_client_destroy (&agent);
        return EXIT_FAILURE;
    }
    zmsg_t *getmsg = computation_message ("TEMPERATURE", name);
    // mlm_client_sendfor () returns zero on success
    if (mlm_client_sendfor (agent, "STATS", "TEMPERATURE", NULL, 0, &getmsg) !=0 ) {
        zsys_error ("Cannot send the message");
        mlm_client_destroy (&agent);
        return 1;
    }

    int i = 0;
    while ( !zsys_interrupted && i < 1 )
    {
        zsys_info ("waiting for %d reply message", i);
        zmsg_t *msg = mlm_client_recv (agent);
        if ( msg == NULL )
            continue;
        if ( streq (mlm_client_command (agent), "MAILBOX DELIVER") )
        {
            char *key = zmsg_popstr (msg);
            char *result = zmsg_popstr (msg);
            zsys_info ("GOT: %s = %s", key , result);
            i++;
            free (key); free (result);
        }
        else {
            zsys_info ("Got a message from %s", mlm_client_command (agent));
        }
        zmsg_destroy (&msg);
    }
    mlm_client_destroy (&agent);
    return 0;
}
