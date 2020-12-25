
#ifndef MYSTMCTR_H_
#define MYSTMCTR_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
struct data {
    char letters[256];
    int isRead[256];
    int read;
    int write;
    int flag;
} ;
void signalHandler( int signum );
char* getRandChar(int times) ;
void del_sem(int sem_id);
int p(int sem_id) ;
int shm_create(key_t key, int m);
void shm_devide(void* shm);
void shm_delete(int shmid);
void* shm_action(int shm_id);
int sem_create(key_t key, int m);
void del_sem(int sem_id);
int p(int sem_id) ;
int v(int sem_id);
#endif // MYSTMCTR_H_INCLUDED


