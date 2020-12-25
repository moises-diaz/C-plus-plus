#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "mystmCtr.h"
#include <csignal>

int shm_id,DP1_id,DP2_id,sem_id;
void *shm = NULL;
int countLetters[20] = {0};
//显示直方图
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

// 获得信号后的处理函数
void signalHandler( int signum ){
    showHisogtam();
    del_sem(sem_id);
    shm_devide(shm);
    shm_delete(shm_id);
    printf("\nShazam !!\n");

    // 清理并关闭
    // 终止程序
    exit(signum);
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    if (argc != 4) {                        // 检查是否正确输入命令
        printf("error way to open dc!");
        exit(EXIT_FAILURE);
    }
    else {
        //printf("open dc!\n");
    }
    int i,j,k;
    for (i=0; i<20; i++) {
        countLetters[i] = 0;                // 初始化字母表中的数目
    }

    struct data* shared;
    shm_id = atoi(argv[1]);                 // 获得共享内存id
    DP1_id = atoi(argv[2]);                 // 获得DP1的PID
    DP2_id = atoi(argv[3]);                 // 获得DP2的PID
    //printf("%d %d %d\n",shm_id,DP1_id,DP2_id);
    sem_id = sem_create((key_t)DP1_id, 1);// 请求信号量，获得其ID，并启用
    while(1){
        shm = shm_action(shm_id);              // 启用分配内存
        if(shm == (void*)-1){
            sleep(10);                           // 如果没找到共享的内存,则停止10s后再找
            continue;
        }
        else{
            shared = (struct data*)shm;
            break;
        }
    }

    while(1) {

        sleep(2);
        p(sem_id);                          // p操作请求获得资源，如果资源已经占用，则挂起当前程序
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
                thisChar = shared->letters[k] - 'A';    // 获得字母对应于数组的数字
                countLetters[thisChar] += 1;             // 计数
                shared->isRead[k] = 1;
                k++;
                if (k > 255){
                    k = 0;
                }
            }

        }
        shared->read = k;
        if (i % 5 == 0){
            showHisogtam();                             // 显示直方图
        }
        i++;
        v(sem_id);                                       // v操作重新释放资源的使用权
    }

    exit(EXIT_SUCCESS);
    return 0;
}




