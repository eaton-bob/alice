#include <malamute.h> 

void main(){
    char *subject, *content;

    mlm_client_t *reader = mlm_client_new ();
    assert (reader);
    int rc = mlm_client_connect (reader, "ipc://@/malamute", 1000, "");
    assert (rc == 0);

    mlm_client_set_consumer (reader, "METRICS", "power.*");


//  Test service pattern
    mlm_client_set_worker (reader, "METRICS_SERVICE", "measurement.*");

    while (!zsys_interrupted) {
        mlm_client_recvx (reader, &subject, &content, NULL);
        printf("reader_cmd:%s,reader_sender:%s,subject:%s,content:%s\n",
	  mlm_client_command (reader),
	  mlm_client_address (reader),
	  subject,
	  content);
        zstr_free (&subject);
        zstr_free (&content);
    }

    mlm_client_destroy (&reader);
}
