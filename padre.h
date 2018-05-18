void padre( char * file); // wrapper

//crea memoria condivisa (gestendo autonomamente i permessi). restituisce -1 se fallisce o il descrittore del file se va a buon fine
struct Shmem attach_segments(int key, int size); 

//utlizza comando shmctl flag IPC_RMID 
void detach_segments(int descrittore); 

//file da caricare (argv[1]), descrittore della shm. esegue open + copia il file 
//nella shm. ritorna -1 se ci sono errori, 1 se va bene
int load_file(char * file, int descrittore); 

//ritorna in byte la dimensione del file
int fileSize(char * file);

/*
int save_key( ancora non saprei )
int check_keys ( boo )
*/
