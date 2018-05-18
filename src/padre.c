#include <stdio.h>
#include <unistd.h>

#include "../include/padre.h"

//wrapper del processo padre
void padre(){
	printf("sono il padre");
}

//crea segmento di memoria condivisa
void attach_segments();

//elimina segmento di memoria condivisa
void detach_segments();

//carica file di input
void load_file();

//salva le chiavi sul file di output
void save_keys();

//ontrolla che le chiavi siano corrette per tutta la lunghezza delle stringhe
void check_keys();
