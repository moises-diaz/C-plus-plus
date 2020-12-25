#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "mystmCtr.h"
#include <string.h>

int main(int argc, char* argv[]){
    if (argc != 2) {                // 检查是否正确输入命令
        printf("error way to open dp-2!");
        exit(EXIT_FAILURE);
    }
    else {
       // printf("open dp-2!\n");
        //fflush(stdout);
    }
    int k;
    int shm_id,sem_id;
    int DP1_pid = getppid();        // 获得父级程序的PID
    int DP2_pid = getpid();         // 获得自己的PID
    char * shm_id_c = argv[1];      // 命令行中的参数写入到变量中
    char DP1_pid_c[20];
    char DP2_pid_c[20];
    char buf[80];  		
    getcwd(buf,sizeof(buf));  	// 获得当前位置的路径
    strcat(buf,"/dc"); 
    void *shm = NULL;
    struct data *shared = NULL;
    sprintf(DP1_pid_c,"%d",DP1_pid);
    sprintf(DP2_pid_c,"%d",DP2_pid);
    shm_id = atoi(argv[1]);
    sem_id = sem_create((key_t) DP1_pid, 1);    // 请求信号量，获得其ID，并启用
    shm = shm_action(shm_id);       // 启用分配内存
    shared = (struct data*)shm;     // 初始化数据结构
    pid_t pid;
    pid = fork();                     // 建立多线程
    if (pid == 0){                    // 父级程序执行DC
         execl(buf,  " ", shm_id_c, DP1_pid_c, DP2_pid_c, (char *)0);
    }
    else{                             // 子级程序执行1/20秒写入1个字母
        while (1) {
            p(sem_id);                // p操作请求获得资源，如果资源已经占用，则挂起当前程序
            k = shared->write;
            if (shared->isRead == 0) {
                continue;
            }
            else{
                char* rc = getRandChar(20);     // 获得随机字母
                shared->letters[k] = rc[k%20];
                //printf("[%c]",shared->letters[k]);
                //fflush(stdout);
                shared->isRead[k] = 0;
                k++;
                if (k > 255){           // 如果k超过了允许的最大内存，则返回到0
                    k = 0;
                }
            }
            shared->write = k;
            v(sem_id);                  // v操作重新释放资源的使用权
            usleep(1000000/20);         // 睡眠1/20s
        }

    }
 //free(shared);

    exit(EXIT_SUCCESS);
    return 0;
}
