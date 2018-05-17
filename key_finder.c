#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define SHMKEY 7777

struct Status{
	int grandson;
	int id_string;
};

int main(int argc, char * argv[]){
	int shmid; //id memoria condivisa		


	if(argc != 2){
		perror("Usage: key_finder <fileInput> <pathOutput>");
		exit(-1);
	}

	/* 	ottengo la dimensione del file per creare una 
		memoria condivisa su misura che contenga 
		file + struct 
	*/
	int fd;
	if((fd = open(argv[1], O_RDONLY, 0660)) == -1){
		perror("Errore nell'apertura del file di input");
		exit(-1);
	}
	struct stat buf;
	fstat(fd, &buf);
	off_t size = buf.st_size; 
	/*    size contiene la grandezza del file (in byte) 
	      il file di input è 275byte
	      printf("%i",(int) size); ---> 275
	*/
	
	if(( shmid = shmget(SHMKEY, sizeof(Status) + (int) size, IPC_CREAT|IPC_EXCL|0660) ) == -1)
		perror("Errore nella creazione della memoria condivisa");
		exit(-1);
	}
	
		

}
