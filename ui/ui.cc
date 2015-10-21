#include <malamute.h>

int main()
{
    const char *endpoint = "ipc://@/malamute";

    mlm_client_t *agent = mlm_client_new ();
    if ( mlm_client_connect (agent, endpoint, 1000, "UI") )
    {
        zsys_info("Cannot connect to the endpoint %s", endpoint);
        mlm_client_destroy (&agent);
        return 1;
    }
    zmsg_t *msg = zmsg_new();
    if (mlm_client_sendfor (agent, "stats", "temperature", NULL, 0, &msg) )
    {
        zsys_info("Cannot send the message");
        mlm_client_destroy (&agent);
        return 1;
    }
    msg = zmsg_new();
    if (mlm_client_sendfor (agent, "stats", "ups.state", NULL, 0, &msg) )
    {
        zsys_info("Cannot send the message");
        mlm_client_destroy (&agent);
        return 1;
    }
    msg = zmsg_new();
    if (mlm_client_sendforx (agent, "stats", "ups.power", NULL, 0, &msg) )
    {
        zsys_info("Cannot send the message");
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
            char *result  = zmsg_popstr (msg);
            zsys_info ("GOT: %s = %s", mlm_client_subject (agent) , result);
            i++;
        }
        else
            continue;
    }
    mlm_client_destroy (&agent);
    return 0;
}
