#include <malamute.h> 

void main(){
    char *subject, *content;
    mlm_client_t *writer = mlm_client_new ();
    assert (writer);
    int rc = mlm_client_connect (writer, "ipc://@/malamute", 1000, "writer");
    assert (rc == 0);
    assert (mlm_client_connected (writer) == true);


    mlm_client_set_producer (writer, "METRICS");


//  Test service pattern

    mlm_client_sendforx (writer, "METRICS_SERVICE", "measurement.power@ups1", "1000W", NULL);
    mlm_client_sendforx (writer, "METRICS_SERVICE", "measurement.th.temperature@rc", "10F", NULL);
    mlm_client_sendforx (writer, "METRICS_SERVICE", "measurement.th.ambiante@rc", "50%", NULL);

    mlm_client_destroy (&writer);
}
