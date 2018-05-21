#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "nipote.h"
#include "types.h"

#define SEMKEY 14
#define MSGKEY 90
#define SHMKEYS1 7777
#define SHMKEYS2 8888


// id shared memory s1 - id shared memory s2 - id del nipote (1 o 2) - id della coda di messaggi
void nipote(int shmid1, int shmid2, int nstrings, int x, int msgid){
	int my_string;
	int sem, shm;
	
	sem = lock();

	//ora sono nella zona in cui posso usare la memoria condivisa
	//la attacco al mio spazio di indirizzamento
	struct Status *p;
	void *addr;
	if(( addr = shmat(shmid1, 0, SHM_R) ) == (void*) -1){
		perror("shmat");
		exit(-1);
	}
	p = addr;
	my_string = p->id_string;
	if(my_string != nstrings){
		//memorizza nel campo Grandson l'id del nipote
		p->grandson = x; //o getpid()?
		p->idstring += 1;
		kill(getppid(), SIGUSR1);
		//unlock()
		sem = unlock();
	}
	else{
		//esco dall'accesso esclusivo 
		//unlock()
		sem = unlock()

		//termina
		if(( shm = shmdet(p) ) == -1){
			perror("shmdet");
			exit(-1);
		}
		exit(0);		
	}
	
	struct spek *stringa = load_string(mystring, addr); //carico la stringa da addr

	char * plain_text[256] = stringa->plain_text;
	char * encoded_text[256] = stringa->encoded_text;

	//trova la chiave
	//e conta i secondi
	clock_t start = clock();
	int seconds = 0;
	unsigned key = find_key(char * plain_text, char * encoded_text)
	clock_t end = clock();
	double timeelapsed = (double) (start - end) / CLOCKS_PER_SEC;

	//salvo key in S2
	save_key(mystring, shmid2, key);
	
	int coda = msgget(MSGKEY, IPC_CREAT|0666);
	
	/*
		deposita il messaggio “chiave
		trovata/secondi” nella coda di messaggi
		del processo logger
	*/
	send_timeelapsed(formattaBene(key), coda); 

	
}

char * formattaBene(unsigned n){
	char * string = "chiave trovata in ";
	char * k = string;
	while(*(k++) != '\0');
	*(k-1) = n + '0';
	return string;
}

struct spek load_string(int mystring, void * addr){
	if(mystring != 0){ // vado a prendere la stringa giusta
		int j = mystring;
		while(j != 0){
			char *p = addr+sizeof(Status);
			while(*p != '\n') p++; //mi sposto fino alla fine della j-esima stringa
			j = j-1; //se ci sono altre stringhe ripeto la procedura
		}
		addr = p;
	}
	else //se è la prima (numero 0) mi sposto solo di struct status
		addr += sizeof(Status);

	char * plain_text[256];
	char * encoded_text[256];
	char * c;
	j = 0;
	c = addr;
	while(*(c+j) != '>'){
		plain_text[j] = *(c+j);
		++j;
	}	
	c = c+j+2;
	j = 0;
	while(*(c+j) != '>'){
		encoded_text[j] = *(c+j);
		++j;
	}
	struct spek *stringa = (struct spek*) malloc(sizeof(struct spek));
	stringa->plain_text = plain_text;
	stringa->encoded_text = encoded_text;
	return stringa;
}

int lock(){
	struct sembuf *s;
	s->sem_num = 0; //un solo semaforo
	s->sem_flg = 0; //nessun flag (così se arriva a 0 si blocca)
	s->sem_op = -1; //decremento il semaforo per utilizzarlo

	if(int sem = semop(SEMKEY, s, 1)) == -1){
		perror("Semop");
		exit(-1);
	}
	return sem;
}

int unlock(){
	struct sembuf *s;
	s->sem_num = 0; 
	s->sem_flg = 0;
	s->sem_op = +1;
	if(sem = semop(SEMKEY, s, 1) == -1){
		perror("semop");
		exit(-1);		
	}
	return sem;
}

unsigned find_key(char * plain_text, char * encoded_text){
	unsigned* plain_unsigned = (unsigned*) plain_text;
	unsigned* encoded_unsigned = (unsigned*) encoded_text;
	unsigned key;
	while(plain_unsigned ^ key != encoded_unsigned) key++;
	return key;
}

void send_timeelapsed(char * string, int coda){
	int messaggio; 
	if((messaggio = msgsnd(coda, string, 19, IPC_NOWAIT)) == -1){
		perror("msgsnd");
		exit(-1);
	}
}

void save_key(int mystring, int shmid2, unsigned key){

	void *shm2;
	if(( shm2 = shmat(shmid2, 0, SHM_W) ) == (void*) -1){
		perror("shmat");
		exit(-1);
	}
	unsigned *in = shm2;
	if(mystring != 0){ // vado a prendere la posizione giusta scorrendo con 'in'
		int k = mystring;	
		while(k != 0){
			in += sizeof(unsigned);
			k = k-1; //se ci sono altre stringhe ripeto la procedura
		}
	}
	*in = key;
}
