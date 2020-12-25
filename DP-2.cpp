#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "mystmCtr.h"
#include <string.h>

int main(int argc, char* argv[]){
    if (argc != 2) {                
        printf("error way to open dp-2!");
        exit(EXIT_FAILURE);
    }
    else {
       // printf("open dp-2!\n");
        //fflush(stdout);
    }
    int k;
    int shm_id,sem_id;
    int DP1_pid = getppid();        
    int DP2_pid = getpid();         
    char * shm_id_c = argv[1];      
    char DP1_pid_c[20];
    char DP2_pid_c[20];
    char buf[80];  		
    getcwd(buf,sizeof(buf));  	
    strcat(buf,"/dc"); 
    void *shm = NULL;
    struct data *shared = NULL;
    sprintf(DP1_pid_c,"%d",DP1_pid);
    sprintf(DP2_pid_c,"%d",DP2_pid);
    shm_id = atoi(argv[1]);
    sem_id = sem_create((key_t) DP1_pid, 1);    
    shm = shm_action(shm_id);       
    shared = (struct data*)shm;     
    pid_t pid;
    pid = fork();                     
    if (pid == 0){                    
         execl(buf,  " ", shm_id_c, DP1_pid_c, DP2_pid_c, (char *)0);
    }
    else{                             
        while (1) {
            p(sem_id);                
            k = shared->write;
            if (shared->isRead == 0) {
                continue;
            }
            else{
                char* rc = getRandChar(20);     
                shared->letters[k] = rc[k%20];
                //printf("[%c]",shared->letters[k]);
                //fflush(stdout);
                shared->isRead[k] = 0;
                k++;
                if (k > 255){           
                    k = 0;
                }
            }
            shared->write = k;
            v(sem_id);                  
            usleep(1000000/20);         
        }

    }
 //free(shared);

    exit(EXIT_SUCCESS);
    return 0;
}
