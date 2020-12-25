#include "mystmCtr.h"

char* getRandChar(int times) {
    char* letter = (char*)malloc(sizeof(char)*times);
    int i;
    srand((unsigned)(getpid() + time(NULL)));
    for (i = 0; i < times; i++){        
        letter[i] = (char)((rand() % 20) + 'A');
    }
    return letter;
}


int shm_create(key_t key, int m)  {
    int shm_id = shmget(key, m, 0666|IPC_CREAT);   
    if (shm_id == -1) {                                   
        printf("shm_create error \n");
        exit(0);
    }
    return shm_id;
}


void* shm_action(int shm_id) {                       
    void *shm = NULL;
	shm = shmat(shm_id, 0, 0);                          
	if(shm == (void*)-1){
		fprintf(stderr, "shm_action failed\n");
		exit(EXIT_FAILURE);
	}
	return shm;
}


void shm_devide(void* shm) {                         
    if(shmdt(shm) == -1){
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
}

void shm_delete(int shmid) {                       
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
}


int sem_create(key_t key, int m) {
	int sem_id;
	sem_id = semget(key, m, 0666 | IPC_CREAT);
	if (sem_id == -1) {
		printf("sem_create error \n");
		exit(0);
	}

	union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *arry;
    } sem_union;

	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1){
		printf("semctl error \n");
		exit(0);
	}

	return sem_id;
}

//删除信号量
void del_sem(int sem_id){
	if (semctl(sem_id, 0, IPC_RMID) == -1) {
        fprintf(stderr, "Failed to delete semaphore \n");
	}
}


int p(int sem_id) {

    struct sembuf sops = {0, -1, SEM_UNDO};	
    int sp = semop(sem_id, &sops,1);
	if(sp == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return sp;
	}
    return sp;
}

int v(int semid) {
    struct sembuf sops = {0,+1,SEM_UNDO};
    return semop(semid,&sops,1);
}
