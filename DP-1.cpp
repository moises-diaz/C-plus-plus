#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "mystmCtr.h"
#include <string.h>

int main() {
    int i,j,k,l;
    int sem_id, shm_id;  
    char shm_id_c[20];   
    void *shm = NULL;    
    char buf[80];  		
    getcwd(buf,sizeof(buf));  	
    strcat(buf,"/dp2"); 
    struct data mydata;
    struct data *shared = (struct data *)malloc(sizeof(mydata)); 
    shm_id = shm_create((key_t)getpid(), sizeof(mydata));  
    sprintf(shm_id_c,"%d",shm_id);
    sem_id = sem_create((key_t)getpid(), 1);  
    shm = shm_action(shm_id);       
    shared = (struct data*)shm;     
    shared->write = 0;
    shared->read = 0;
    shared->flag = 1;
    for (i=0; i<20; i++) {
        shared->isRead[i] = -1;
    }
    pid_t pid;
    pid = fork();                     
    if(pid == 0){                     
        execl(buf, " ", shm_id_c, (char *)0);
    }
    else{                             
        while(1) {
            p(sem_id);                
            k = shared->write;
            for (j = 0; j < 20; j++){
                if (shared->isRead == 0) {
                    break;
                }
                shared->isRead[k] = 0;
                k++;
                if (k > 255){           
                    k = 0;
                }
            }
            if (j > 0) {
                char* rc = NULL;
                rc = getRandChar(j);   
                //printf("{");
                //fflush(stdout);
                for (l=0;l<j;l++){
                    shared->letters[l] = rc[l];
                    //printf("%c",shared->letters[l]);
                    //fflush(stdout);
                }
                //printf("}");
                //fflush(stdout);
            }
            shared->write = k;
            v(sem_id);                  
            sleep(2);
        }
    }
    //free(shared);
    printf("out!");
    exit(EXIT_SUCCESS);
    return 0;
}
