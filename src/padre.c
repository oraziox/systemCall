#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#include "padre.h"
#include "types.h"
#include "nipote.h"

#define SHMKEYS1 7777
#define SHMKEYS2 8888
#define BUFSIZE 1024

void padre(char * file, char * fileOutput){
	struct Shmem s1, s2; //strutture in types.h
	
	pid_t pid1;
	pid_t pid2;	

	s1 = attach_segments(SHMKEYS1, sizeof(Status) + fileSize(file) );

	load_file(file, s1->addr); //void ? 

	s2 = attach_segments(SHMKEYS2, 32*sizeof(byte)); // quanta memoria ?

	if( ( pid1 = fork() ) == -1 ){ //logger
		perror("Errore nella fork");
		exit(-1);
	}
	else if (pid1 == 0)
		logger();
	
	if( ( pid2 = fork() ) == -1 ){ //figlio
		perror("Errore nella fork");
		exit(-1);
	}
	else if (pid2 == 0)
		figlio();
	
	wait();

	int mystring = 3; // andrebbe fatta globale
	if(check_keys(s2->addr))
		save_keys(fileOutput);
	
	detach_segments(s1->shmid);
	detach_segments(s2->shmid);
	exit(0);
}
//*/*/*/*//*/**/*/*/*/*/*/*/mettere nelle funzioni utili*/*/*/*/*/*/*/*/*/*/*/*/
int fileSize(char * file){
	struct stat buf;
	fstat(fd, &buf);
	off_t size = buf.st_size; 
	return (int) size;
}

char * hex(int n){
	char * hex = (char *) malloc(sizeof(char) * 100);

	int i = 0;
	while(n!=0){
		int temp = 0;
		temp = n%16;
		if(temp < 10){
			hex[i] = temp + 48;
			i++;
		}
		else{
			hex[i] = temp + 55;
			i++;
		}
		n = n/16;
	}
	char * hexx = (char *) malloc(sizeof(char) * i);
	for(int j = i-1, int k = 0; j >= 0; j--, k++){
		hexx[k] = hex[j]; 
	}
	free(hex);
	return hexx;
}


//*/*/*/*//*/**/*/*/*/*/*/*/mettere nelle funzioni utili*/*/*/*/*/*/*/*/*/*/*/*/

struct Shmem attach_segments(key_t key, int size){ 
	struct Shmem s; //struttura risultato definita in types.h

	int shmid;
	
	if(shmid = shmget(key, size, IPC_CREAT|IPC_EXCL|0660)) == -1)
		s->shmid = shmid;
		s->addr = (void*) 0;
		return s;
	else{
		void* a;		
		a = shmat(shmid, 0, SHM_W);
		if( a == (void*) -1)
			s->shmid = 0;
			s->addr = a;
			return s;
		else{
			s->shmid = shmid;
			s->addr = a;
			return s;
		}
	}
}

void detach_segments(int descrittore){
	while(!shmctl(descrittore, IPC_RMID, NULL));
}

void load_file(char * file, void* addr){ //ci sono da gestire gli errori
	int fd = open(file, O_RDWR, 0666);
	int size;
	char * buf = (char*) malloc( BUFSIZE );
	struct Status *s;
	*addr = *s;
	do{
		size = read(fd, buf, BUFSIZE); //leggo dal file
		strcpy(addr, buf); //copio quello che ho letto nella shmid
		
	}while (size > 0) //finchè c'è da leggere, read restituisce i byte letti
	free(buf);
	close(fd);
}

bool check_keys(int i, const int max, void * addr){
	if( i < max ){
		struct Spek *string = load_string(i, addr);
		unsigned key = load_key(i, addr);
		if(string->plain_text ^ key == string->encoded_text)
			return check_keys(i+1, addr);
		else
			return false
	}	
	return true;
}

unsigned load_key(int i, void * addr){
	unsigned key;
	unsigned * in;
	in = addr;
	
	while(i != 0){
		in += sizeof(unsigned);
		i = i-1;
	}
	key = *in;
	return key;	
}

void save_keys(char * fileOutput, void * addr){
	int fd = open(fileOutput, O_RDWR, 0666);
	int j = 0;
	while(j <= mystring){
		unsigned key = load_key(j, addr);
		char * keyHexed = hex(key);
		int wr = write(fd, keyHexed, 32);
		j++;
	}
	close(fd);
}
