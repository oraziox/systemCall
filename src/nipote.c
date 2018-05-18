#include "../include/nipote.h"

//wrapper del processo nipote
void nipote();

//legge la stringa dal segmento S1
void load_string();

//blocca accesso esclusivo regione critica
void lock();

//sblocca accesso esclusivo regione critica
void unlock();

//trova la chiave
void find_key();

//deposita il messaggio "chiave trovate/secondi" nella coda di messaggi del processo logger
void send_timeelapsed();

//salva la chiave nel segmento S2
void save_key();

