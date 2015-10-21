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
    zmsg_t *getmsg = zmsg_new();
    zmsg_addstr(getmsg, "GET");
    zmsg_addstr(getmsg, "temperature");
    if (mlm_client_sendfor (agent, "stats", "temperature", NULL, 0, &getmsg) )
    {
        zsys_info("Cannot send the message");
        mlm_client_destroy (&agent);
        return 1;
    }
    getmsg = zmsg_new();
    zmsg_addstr(getmsg, "GET");
    zmsg_addstr(getmsg, "ups.state");
    if (mlm_client_sendfor (agent, "stats", "ups.state", NULL, 0, &getmsg) )
    {
        zsys_info("Cannot send the message");
        mlm_client_destroy (&agent);
        return 1;
    }
    getmsg = zmsg_new();
    zmsg_addstr(getmsg, "GET");
    zmsg_addstr(getmsg, "ups.power");
    if (mlm_client_sendforx (agent, "stats", "ups.power", NULL, 0, &getmsg) )
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
            char *key = zmsg_popstr (msg);
            if ( ( strcmp(key, "temperature") == 0 ) ||
                 ( strcmp(key, "ups.power") == 0 ) ||
                 ( strcmp(key, "ups.temperature") == 0 ) )
            {
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
