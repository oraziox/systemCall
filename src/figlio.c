#include "../include/figlio.h"

//wrapper del processo figlio
void figlio();

//signal handler
void status_updated();

//deposita il messaggio di terminazione nella coda di messaggi del processo logger
void send_terminate();
