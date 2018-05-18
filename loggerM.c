#include <sys/ipc.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../include/logger.h"
#include "../include/types.h"

#define MSGKEY   90		/* chiave per la coda */
//#define MSGTYPE  1		/* tipo per il messaggio da ricevere */



int msgid;		/* identificatore della coda */

void exit_fnc(){
	//DEBUG
	int mask = 0666;
	msgid = msgget(MSGKEY, mask);
	msgctl(msgid, IPC_RMID, NULL);
	exit(1);
}

//wrapper del processo logger
void logger(){
	
	int mask = 0666;
	msgid = msgget(MSGKEY, mask);
	msgctl(msgid, IPC_RMID, NULL);
	
	signal(SIGALRM, exit_fnc);
	
	//debug!!

	
	/*
		puntatore alla struttura che rappresenta il messaggio.
		Il messaggio è definito nel file def.h
	*/
	struct Message *m;

	/* allocazione memoria per il messaggio */
	m = (struct Message *) malloc(sizeof(struct Message));

	/*
		Creazione della coda di messaggi.
		La flag 0666|IPC_CREAT|IPC_EXCL fa sì che la
		coda venga creata, con permessi di lettura e
		scrittura per tutti, solo se non esiste già
		un'altra coda associata alla chiave MSGKEY.
		Vedere man msgget
	*/
	if((msgid = msgget(MSGKEY, (0666|IPC_CREAT|IPC_EXCL))) == -1) {
		perror("Creazione della coda di messaggi fallita");
		exit(1);
	}
	
	const unsigned msg_size = sizeof(struct Message) - sizeof(long);
	
	int quit = 0;
	
	/*
		Il processo si mette in attesa per leggere dalla coda
	*/
	alarm(120);
	while(quit != 1) {
		//polling di un secondo
		sleep(10);
		quit = polling_receive(msgid, m, msg_size);
	}
	
	exit(1);
};

//scarica la coda di messaggi e la stampa su stdout
int polling_receive(int msgid, struct Message *m, const unsigned msg_size){
	printf("Sono nel receive\n");
	/*
			la system call msgrcv riceve i messaggi dalla coda msgid e li
			memorizza nel buffer m.
			Il terzo parametro della funzione rappresenta la dimensione
			del messaggio ad esclusione dello spazio necessario a contenere
			il campo mtype della struttura.
			Il quarto parametro indica che devono essere letti solo i messaggi
			con message type MSGTYPE.
			L'ultimo parametro indica che il processo deve rimanere in attesa
			se non ci sono messaggi nella coda.
			Vedere man msgrcv.
		*/
		while(msgrcv(msgid, m, msg_size, 0, IPC_NOWAIT) != -1) {
			write(1, m->text, msg_size);
			write(1, "\n", 1);
			if(m->mtype == 1){
				//for debugging
				msgctl(msgid, IPC_RMID, NULL);
				printf("Ricevuto messaggio finale!\n");				
				return 1;
			}
		}
} 

int main(int argc, char *argv[]) {
	logger();
}
