#include <malamute.h>
#include <assert.h>

static char*
s_compute_average (zlistx_t *list) {

    int sum = 0;

    void *it;
    for (it = zlistx_first (list); it != NULL; it = zlistx_next (list)) {
        sum += atoi ((char*) it);
    }

    double avg = sum / zlistx_size (list);

    char *ret;
    asprintf (&ret, "%f", avg);
    return ret;
}

int main() {

    //static const char* ENDPOINT = "tcp://192.168.199.104:9999";
    static const char* ENDPOINT = "ipc://@/malamute";

    mlm_client_t *client = mlm_client_new ();
    assert (client);

    int r = mlm_client_connect (client, ENDPOINT, 5000, __FILE__);
    assert (r != -1);

    r = mlm_client_set_worker (client, "STATS", "TEMPERATURE.*");
    assert (r != -1);

    r = mlm_client_set_consumer (client, "METRIC", "TEMPERATURE.*");
    assert (r != -1);

    zlistx_t *list = zlistx_new();
    assert (list);
    // Use item handlers
    zlistx_set_destructor (list, (czmq_destructor *) zstr_free);
    zlistx_set_duplicator (list, (czmq_duplicator *) strdup);
    zlistx_set_comparator (list, (czmq_comparator *) strcmp);

    while (!zsys_interrupted) {

        zmsg_t *msg = mlm_client_recv (client);

        if (streq (mlm_client_command (client), "MALIBOX DELIVER"))
            goto msg_destroy;
        else
        if (streq (mlm_client_command (client), "STREAM DELIVER")) {
            char *name = zmsg_popstr (msg);
            char *type = zmsg_popstr (msg);
            char *value = zmsg_popstr (msg);
            zlistx_add_end (list, value);
            zstr_destroy (&name);
            zstr_destroy (&type);
            zstr_free (&value);
            goto msg_destroy;
        }
        else
        if (streq (mlm_client_command (client), "SERVICE DELIVER")) {
            char *result = s_compute_average (list);
            zmsg_t *reply = zmsg_new ();
            zmsg_addstr (reply, result);
            mlm_client_sendto (
                client,
                mlm_client_sender (client),
                "TEMPERATURE.AVERAGE",
                NULL,
                5000,
                &reply);
            zstr_free (&result);
        }

msg_destroy:
        zmsg_destroy (&msg);
    }

    zlistx_destroy (&list);
    mlm_client_destroy (&client);

}
