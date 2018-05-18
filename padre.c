#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "padre.h"
#include "types.h"

#define SHMKEYS1 7777
#define SHMKEYS2 8888
#define BUFSIZE 1024

void padre(char * file){
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
		execl( ... )
	
	
	if( ( pid2 = fork() ) == -1 ){ //figlio
		perror("Errore nella fork");
		exit(-1);
	}
	else if (pid2 == 0)
		execl( ... )
	
	wait();
	if(check_keys())
		save_keys();
	
	detach_segments(s1->shmid);
	detach_segments(s2->shmid);
}

int fileSize(char * file){
	struct stat buf;
	fstat(fd, &buf);
	off_t size = buf.st_size; 
	return (int) size;
}

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
	
	do{
		size = read(fd, buf, BUFSIZE); //leggo dal file
		strcpy(addr, buf); //copio quello che ho letto nella shmid
		
	}while (size > 0) //finchè c'è da leggere, read restituisce i byte letti

	close(fd);
}
