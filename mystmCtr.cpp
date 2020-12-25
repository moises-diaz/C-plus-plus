#include "mystmCtr.h"
//得到A～T的一个随机字母
char* getRandChar(int times) {
    char* letter = (char*)malloc(sizeof(char)*times);
    int i;
    srand((unsigned)(getpid() + time(NULL)));
    for (i = 0; i < times; i++){        // 根据随机的次数随机大小为n的char*
        letter[i] = (char)((rand() % 20) + 'A');
    }
    return letter;
}

//生成共享内存
int shm_create(key_t key, int m)  {
    int shm_id = shmget(key, m, 0666|IPC_CREAT);   // key为键值,一般可以用程序的PID作为唯一标识
    if (shm_id == -1) {                                   // 0666是权限信息,IPC_CREAT是有则提取出shmID，没有这个内存则创建一个内存
        printf("shm_create error \n");
        exit(0);
    }
    return shm_id;
}

// 启动共享内存
void* shm_action(int shm_id) {                       // 内存只有被启动了才能使用
    void *shm = NULL;
	shm = shmat(shm_id, 0, 0);                          // 后两个参数一般都为0
	if(shm == (void*)-1){
		fprintf(stderr, "shm_action failed\n");
		exit(EXIT_FAILURE);
	}
	return shm;
}

//分离共享内存
void shm_devide(void* shm) {                         // 是分离不是删除,分离是不使用这些内存了,还给系统
    if(shmdt(shm) == -1){
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
}
//删除共享内存
void shm_delete(int shmid) {                        // 这里的删除共享内春是指不再使用这个shmid下的共享内存.
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
}

// 生成信号量i
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

//p操作
int p(int sem_id) {
/*
sembuf第2个参数sem_op：
如果其值为正数，该值会加到现有的信号内含值中。通常用于释放所控资源的使用权；
如果sem_op的值为负数，而其绝对值又大于信号的现值，操作将会阻塞，直到信号值大于或等于sem_op的绝对值。通常用于获取资源的使用权；
如果sem_op的值为0，则操作将暂时阻塞，直到信号的值变为0。
*/
    struct sembuf sops = {0, -1, SEM_UNDO};	// sops数组大小为1
    int sp = semop(sem_id, &sops,1);
	if(sp == -1)
	{
		fprintf(stderr, "semaphore_p failed\n");
		return sp;
	}
    return sp;
}

//v操作
int v(int semid) {
    struct sembuf sops = {0,+1,SEM_UNDO};
    return semop(semid,&sops,1);
}
