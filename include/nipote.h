#ifndef NIPOTE_H
#define NIPOTE_H

//wrapper del processo nipote
// id shared memory s1 - id shared memory s2 - id del nipote (1 o 2) - id della coda di messaggi
void nipote(int shmid1, int shmid2, int nstrings, int x, int msgid);

//legge la stringa dal segmento S1
struct Spek load_string(int mystring, void * addr);

//blocca accesso esclusivo regione critica
int lock();

//sblocca accesso esclusivo regione critica
int unlock();

//trova la chiave
unsigned find_key(char * plain_text, char * encoded_text);

//deposita il messaggio "chiave trovate/secondi" nella coda di messaggi del processo logger
void send_timeelapsed(char * string, int coda);

//salva la chiave nel segmento S2
void save_key(int mystring, int shmid2, unsigned key);

#endif
