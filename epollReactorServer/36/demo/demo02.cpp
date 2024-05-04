/*
 * 程序名：demo02.cpp，此程序用于演示无名管道。
 * 作者：吴从周
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>          
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>

int main(int argc,char *argv[])
{
    int pipefd[2];                         // 无名管道，有两个fd，表示管道的两端，fd[0]用于读，fd[1]用于写。
    if (pipe2(pipefd,0)==-1)       // 创建无名管道。  O_CLOEXEC|O_NONBLOCK
    {
        printf("创建管道失败。\n"); return -1;
    }

    write(pipefd[1], "hello world", 11);            // 往管道中写数据。
 
    ssize_t ret;
    char buf[128]={0};

    ret = read(pipefd[0], &buf, 5);    // 从管道中读取数据。
    printf("ret=%d,buf=%s\n",ret,buf);

    ret = read(pipefd[0], &buf, sizeof(buf));    // 从管道中读取数据。
    printf("ret=%d,buf=%s\n",ret,buf);

    close(pipefd[0]);   // 关闭管道的读。
    close(pipefd[1]);   // 关闭管道的写。

    return 0;
}