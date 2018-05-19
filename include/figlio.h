#ifndef FIGLIO_H
#define FIGLIO_H

//wrapper del processo figlio
void figlio(int s1Size);

//signal handler
void status_updated(int currentSignal);

//deposita il messaggio di terminazione nella coda di messaggi del processo logger
void send_terminate();

#endif
