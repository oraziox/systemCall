#include <stdio.h>
#include <unistd.h>

#include "../include/padre.h"

int main(int argc, int **argv){
	
	if(argc != 3){
		printf("errore argomenti\n");
	}else{
		padre();
	}	
	
	return 0;
}
