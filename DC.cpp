#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "mystmCtr.h"
#include <csignal>

int shm_id,DP1_id,DP2_id,sem_id;
void *shm = NULL;
int countLetters[20] = {0};

void showHisogtam() {
    printf("\033c") ;
    fflush(stdout);
    int i,j;
    int o,t,h;
    for (i=0;i<20;i++){
        o =  countLetters[i] % 10;
        t =  countLetters[i] / 10 % 10;
        h = countLetters[i] / 100;
        if(countLetters[i] < 10){
            printf("%c-00%d ",i+65,countLetters[i]);
            fflush(stdout);
        }
        else if(countLetters[i] >= 10 && countLetters[i] < 100){
            printf("%c-0%d ",i+65,countLetters[i]);
            fflush(stdout);
        }
        else{
            printf("%c-%d ",i+65,countLetters[i]);
            fflush(stdout);
        }
        fflush(stdout);
       for (j=0; j<h; j++){
            printf("*");
            fflush(stdout);
       }
       for (j=0; j<t; j++){
            printf("+");
            fflush(stdout);
       }
       for (j=0; j<o; j++){
            printf("-");
            fflush(stdout);
       }
       printf("\n");
       fflush(stdout);
    }
}


void signalHandler( int signum ){
    showHisogtam();
    del_sem(sem_id);
    shm_devide(shm);
    shm_delete(shm_id);
    printf("\nShazam !!\n");


    exit(signum);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    if (argc != 4) {                        
        printf("error way to open dc!");
        exit(EXIT_FAILURE);
    }
    else {
        //printf("open dc!\n");
    }
    int i,j,k;
    for (i=0; i<20; i++) {
        countLetters[i] = 0;                
    }

    struct data* shared;
    shm_id = atoi(argv[1]);                 
    DP1_id = atoi(argv[2]);                
    DP2_id = atoi(argv[3]);                 
    //printf("%d %d %d\n",shm_id,DP1_id,DP2_id);
    sem_id = sem_create((key_t)DP1_id, 1);// 
    while(1){
        shm = shm_action(shm_id);              
        if(shm == (void*)-1){
            sleep(10);                           
            continue;
        }
        else{
            shared = (struct data*)shm;
            break;
        }
    }

    while(1) {

        sleep(2);
        p(sem_id);                         
        /*for (j = 0; j < 60; j++){
            printf("[%d]",shared->isRead[j]);
            fflush(stdout);
        }*/
        k = shared->read;
        int thisChar;
        for (j = 0; j < 60; j++){
            if(shared->isRead[k] == 1){
                break;
            }
            if(shared->isRead[k] != -1){
                thisChar = shared->letters[k] - 'A';    
                countLetters[thisChar] += 1;             
                shared->isRead[k] = 1;
                k++;
                if (k > 255){
                    k = 0;
                }
            }

        }
        shared->read = k;
        if (i % 5 == 0){
            showHisogtam();                             
        }
        i++;
        v(sem_id);                                       
    }

    exit(EXIT_SUCCESS);
    return 0;
}




