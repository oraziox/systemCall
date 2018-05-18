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
