struct Status{
	int grandson;
	int id_string;
};

struct Message{
	long mtype;
	char text[128];
};

struct Shmem{
	int shmid;	//id memoria condivisa
	void* addr; 	//spazio indirizzamento
};

struct sembuf{
	ushort	sem_num;
	short	sem_op;
	short	sem_flg;
}

struct Spek{ 
	char * plain_text[256];
	char * encoded_text[256];
}	
