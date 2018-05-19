//#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>		//malloc, exit
#include <stdio.h>		//perror, printf, sprintf
#include <wait.h>			//SIGUSR1, signal
#include <unistd.h>		//fork, pause, write
#include <string.h>		//strcpy

#include "../include/figlio.h"
#include "../include/types.h"
#include "../include/nipote.h"

#define MSGKEY   90
#define SHMKEYS1 7777
#define SHMKEYS2 8888

#define SEMKEY 14




//						DEBUG

struct Test {
	int id;
	int sid;
};

//test per mem condivisa da signal
void signalTry();




// 						EXTRA

//dato un intero ne calcola le cifre
int countCifre(int i);




//						CONSEGNA

//deposita il messaggio di terminazione nella coda di messaggi del processo logger
void send_terminate();

//signal handler del segnale SIGUSR1
void status_updated(int currentSignal);




//						VARIABILI GLOBALI

int shmid;					//id della shmem
int size;						//size della struttura Status
char *shm;					//shmem address
struct Status *st; 	//puntatore allo status in memoria

pid_t pid1, pid2;		//pid per nipoti
int semid;					//id del semaforo


struct Test *t;



int main(){
	
	/* Testing Shared Memory
	
	size=sizeof(struct Test);

	shmid = shmget(SHMKEYS1, 2*size, IPC_CREAT|0666);
	shm=shmat(shmid, NULL, 0);

	t = (struct Test *)shm + (2*size-size);
	t->id = 451;
	t->sid = 993;
	signalTry();
		
	shmdt(shm);
	printf("Detached\n");
	*/
	
	/* Testing Semaphore */
	
	
	struct sembuf* sops = (struct sembuf *) malloc(sizeof(struct sembuf));
	if ((semid = semget(SEMKEY, 2,IPC_CREAT|IPC_EXCL|0666)) == -1) {
		perror ("semget");
		exit (1);
	}
	
	printf("Creato Semaforo\n");
	
	//setta il semaforo a 1 (mutex)
	sops->sem_num = 0;
	sops->sem_op = 1;
	sops->sem_flg = 0;
	
	semop(semid, sops, 1);
	printf("Settato semaforo a 1\n");
	
	//Rimozione semaforo
	int v;
	v = semctl(semid,0, GETVAL);
	printf("Il semaforo vale: %i\n", v);
	semctl(semid, 0, IPC_RMID);
	free((struct sembuf*)sops);	
	printf("Distrutto semaforo\n");	
	
	return 0;
}


//wrapper del processo figlio, riceve in input la size del buffer s1
void figlio(int s1Size){
	
	int status1, status2;
	
	//Setup zona condivisa
	size = sizeof(struct Status);
	//Ricerca zona condivisa
	if ((shmid = shmget(SHMKEYS1, s1Size, 0666)) < 0) {
        perror("shmget");
        exit(1);
  }//Attach zona condivisa
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
  }
			
	//Creazione semaforo per figli
	int semid;
	
	struct sembuf* sops = (struct sembuf *) malloc(sizeof(struct sembuf));
	if ((semid = semget(SEMKEY, 2,IPC_CREAT|IPC_EXCL|0666)) == -1) {
		perror ("semget");
		free((struct sembuf*)sops);//libero malloc semaforo
		exit (1);
	}
	
	//setta il semaforo a 1 (mutex)
	sops->sem_num = 0;
	sops->sem_op = 1;
	sops->sem_flg = 0;
	
	semop(semid, sops, 1);
	
	//si registra per SIGUSR1
	signal(SIGUSR1, status_updated);
	
	/*
		Crea i vari nipoti
	*/
	pid1 = fork(); //Primo nipote
	if(pid1 == -1){
		perror("nipote1");
		free((struct sembuf*)sops);//libero malloc semaforo
		exit(1);
	}
	
	if(pid1 == 0){	//Se sono il nipote appena creato
		//nipote(semid);
		exit(0);
	}		
	//se sono il padre
	pid2 = fork(); //Secondo nipote
	if(pid2 == -1){
		perror("nipote2");
		free((struct sembuf*)sops);//libero la malloc
		exit(1);
	}
	
	if(pid2 == 0){	//Se sono il secondo nipote appena creato
		//nipote();
		exit(0);
	}
	
	waitpid(pid1, &status1, 0);
	if(WEXITSTATUS(status1) == 1){
		write(1, "Nipote 1 crashato.\n", 19);
	}
	
	waitpid(pid2, &status2, 0);
	if(WEXITSTATUS(status2) == 1){
		write(1, "Nipote 2 crashato.\n", 19);
	}
	
	//Detach della zona di memoria condivisa
	if(shmdt(shm) == -1){
		perror("shmdt");
		free((struct sembuf*)sops);//libero la malloc prima di uscire
		exit(1);
	}						
	printf("Detached\n");		//DEBUG
	
	//Rimozione semaforo
	semctl(semid, 0, IPC_RMID);
	free((struct sembuf*)sops);
		
	send_terminate();
}







//signal handler
void status_updated(int currentSignal){
	int ts= sizeof(char)*(46+countCifre(st->grandson)+countCifre(st->id_string));
	char c[ts];
	sprintf(c, "Il nipote %i sta analizzando la %i-esima stringa.\n", t->id, t->sid);
	write(1, c, ts);
}

void send_terminate(){

	int msgid;
	
	/*
		puntatore alla struttura che rappresenta il messaggio.
		Il messaggio è definito nel file types.h
	*/
	struct Message *m;

	/* allocazione memoria per il messaggio */
	m = (struct Message *) malloc(sizeof(struct Message));
	
	if((msgid = msgget(MSGKEY, 0666)) == -1) {
		perror("Coda di messaggi non esistente");
		free((struct Message *)m);//libero la malloc prima di uscire
		exit(1);
	}
	
	const unsigned msg_size = sizeof(struct Message) - sizeof(long);
	/* creazione del messaggio da inviare */
	m->mtype = 1;
	strcpy(m->text, "ricerca conclusa");
	//Mando il messaggio
	if(msgsnd(msgid, m, sizeof(struct Message) - sizeof(m->mtype), 0) == -1) {
			perror("msgsnd");
			free((struct Message *)m);//libero la malloc prima di uscire
			exit(1);
	}
	free((struct Message *)m);//libero la malloc prima di uscire
}

int countCifre(int i){
	int c = 0;
	while (i!=0){
		c++;
		i=i/10;
	}
	return c;
}

void signalTry(){
	printf("%i\n", t->id);
}
