#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include "../include/types.h"

#define MSGKEY   90		/* chiave per la coda */
//#define MSGTYPE  1		/* tipo per il messaggio da inviare */


int main(int argc, char *argv[]) {

	/*
		puntatore alla struttura che rappresenta il messaggio.
		Il messaggio è definito nel file def.h
	*/
	struct Message *m;

	int msgid;		/* identificatore della coda */

	char c = 'S';

	/* allocazione memoria per il messaggio */
	m = (struct Message *) malloc(sizeof(struct Message));

	/*
		apertura  della coda di messaggi corrispondente
		alla chiave MSGKEY
	*/
	if((msgid = msgget(MSGKEY, 0666)) == -1) {
		perror("Apertura della coda di messaggi fallita.\n");
		exit(1);
	}

	int l=0;

	/*
		il processo cicla finchè l'utente vuole
		inviare nuovi messaggi al server
	*/
	while(c == 'S' || c == 's') {
		
		long int tp;
		printf("Scegliere il tipo di messaggio: ");
		scanf("%li", &tp);
		/* creazione del messaggio da inviare */
		m->mtype = tp;
		printf("tp vale: %li\n", m->mtype);
		
		printf("Scrivo: ciao %i\n", l);
		strcpy(m->text, "ciao");
		/*
			la system call msgsnd spedisce il messaggio
			memorizzato nel buffer m verso la coda msgid.
			Il terzo parametro della funzione rappresenta la dimensione
			del messaggio ad esclusione dello spazio necessario a contenere
			il campo mtype della struttura.
			L'ultimo parametro indica che il processo deve rimanere in attesa
			se la coda è piena finche' si libera spazio per inserire il
			messaggio.
			Vedere man msgsnd.
		*/
		if(msgsnd(msgid, m, sizeof(struct Message) - sizeof(m->mtype), 0) == -1) {
			perror("msgsnd");
			exit(1);
		};

		/*
			all'utente viene chiesto se vuole inviare
			un nuovo messaggio
		*/
		do {
			c = getchar();
			printf("Altro messaggio? [S/N] ");
			c = getchar();
		} while (c != 's' && c != 'n' && c != 'S' && c != 'N');

	}

	return 0;
}
