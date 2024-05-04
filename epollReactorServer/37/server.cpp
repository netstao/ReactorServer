/*
 * 程序名：tcpepoll.cpp，此程序用于演示采用epoll模型实现网络通讯的服务端。
*/
/*
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
#include <netinet/tcp.h>      // TCP_NODELAY需要包含这个头文件。
#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"
#include "Channel.h"
#include "EventLoop.h"
*/
#include "EchoServer.h"
#include <signal.h>

 //设置2和15的信号 ， 在信号处理函数中停止主从事件循环 3程序主动退出

EchoServer *server;

void Stop(int sig)
{
  printf("sig=%d\n",sig);
  printf("server stoping...\n");
  server->stop();
  delete server;
  printf("delete server.\n");
  exit(0);
}

int main(int argc,char *argv[])
{
  if (argc != 3) 
  { 
      printf("usage: ./tcpepoll ip port\n"); 
      printf("example: ./tcpepoll 192.168.150.128 5085\n\n"); 
      return -1; 
  }

  signal(SIGTERM,Stop);
  signal(SIGINT,Stop);

  server = new EchoServer(argv[1],atoi(argv[2]),64,0);
  server->start();


  return 0;
}