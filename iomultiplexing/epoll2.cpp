/*
* epoll2.cpp，此程序用于演示采用epoll模型的边缘触发。
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

// 把socket设置成非阻塞。
int setnonblocking(int fd)
{
  int flags;

  // 获取fd的状态。
  if ((flags=fcntl(fd,F_GETFL,0))==-1)
    flags = 0;

  return fcntl(fd,F_SETFL,flags|O_NONBLOCK);
}

// 初始化服务端的监听端口。
int initserver(int port);

int main(int argc,char *argv[])
{
  if (argc != 2) { printf("usage: ./tcpepoll2 port\n"); return -1; }

  // 初始化服务端用于监听的socket。
  int listensock = initserver(atoi(argv[1]));
  printf("listensock=%d\n",listensock);

  if (listensock < 0) { printf("initserver() failed.\n"); return -1; }

  setnonblocking(listensock);       // 把listensock设置为非阻塞。

  // 创建epoll句柄。
  int epollfd=epoll_create(1);

  // 为服务端的listensock准备读事件。
  epoll_event ev;       // 声明事件的数据结构。
  ev.data.fd=listensock;  // 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回。
  // ev.data.ptr=(void*)"超女";  // 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回。
  ev.events=EPOLLIN;   // 打算让epoll监视listensock的读事件，LT（水平）模式。
  // ev.events=EPOLLIN | EPOLLET;   // 打算让epoll监视listensock的读事件，ET（边缘）模式。

  epoll_ctl(epollfd,EPOLL_CTL_ADD,listensock,&ev);   // 把需要监视的socket和事件加入epollfd中。

  epoll_event evs[10];   // 存放epoll返回的事件。

  while (true)    // 事件循环。
  {
    // 等待监视的socket有事件发生。
    int infds=epoll_wait(epollfd,evs,10,-1);

    // 返回失败。
    if (infds < 0)
    {
      perror("epoll() failed"); break;
    }

    // 超时。
    if (infds == 0)
    {
      printf("epoll() timeout.\n"); continue;
    }

    // 如果infds>0，表示有事件发生的socket的数量。
    for (int ii=0;ii<infds;ii++)    // 遍历epoll返回的数组evs。
    {
      // printf("ptr=%s,events=%d\n",evs[ii].data.ptr,evs[ii].events);

      // 如果发生事件的是listensock，表示有新的客户端连上来。
      if (evs[ii].data.fd==listensock)
      {
        while (true)
        {
          struct sockaddr_in client;
          socklen_t len = sizeof(client);
          int clientsock = accept(listensock,(struct sockaddr*)&client,&len);
          if ( (clientsock<0) && (errno==EAGAIN) ) break;

          printf ("accept client(socket=%d) ok.\n",clientsock);

          // 为新客户端准备读事件，并添加到epoll中。
          setnonblocking(clientsock);       // 把客户端连接的socket设置为非阻塞。
          ev.data.fd=clientsock;
          // ev.events=EPOLLOUT;            // LT-水平触发。
          ev.events=EPOLLOUT|EPOLLET;    // ET-边缘触发。
          epoll_ctl(epollfd,EPOLL_CTL_ADD,clientsock,&ev);
        }
      }
      else
      {
        printf("触发了写件事。\n");
        for (int ii=0;ii<10000000;ii++)
        {
          if (send(ev.data.fd,"aaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbb",30,0)<=0)
          {
            if (errno==EAGAIN) 
            { 
              printf("发送缓冲区已填懣。\n"); break; 
            }
          }
        }
        /*
        // 如果是客户端连接的socke有事件，表示有报文发过来或者连接已断开。
        char buffer[1024];    // 存放从客户端读取的数据。
        memset(buffer,0,sizeof(buffer));
        int  readn;         // 每次调用recv()的返回值。
        char *ptr=buffer;    // buffer的位置指针。
        while (true)
        {
          if ( (readn=recv(evs[ii].data.fd,ptr,5,0))<=0 )
          {
            if ( (readn<0) && (errno==EAGAIN) ) 
            {  // 如果数据被读取完了，把接收到的报文内容原封不动的发回去。
              send(evs[ii].data.fd,buffer,strlen(buffer),0);
              printf("recv(eventfd=%d):%s\n",evs[ii].data.fd,buffer);
            }
            else
            {
              // 如果客户端的连接已断开。
              printf("client(eventfd=%d) disconnected.\n",evs[ii].data.fd);
              close(evs[ii].data.fd);      // 关闭客户端的socket
            }

            break;    // 跳出循环。
          }
          else
            ptr=ptr+readn;          // buffer的位置指针后移。 
        }
        */
      }
    }
  }

 return 0;
}

// 初始化服务端的监听端口。
int initserver(int port)
{
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if (sock < 0)
  {
    perror("socket() failed"); return -1;
  }

  int opt = 1; unsigned int len = sizeof(opt);
  setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,len);

  struct sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if (bind(sock,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
  {
    perror("bind() failed"); close(sock); return -1;
  }

  if (listen(sock,5) != 0 )
  {
    perror("listen() failed"); close(sock); return -1;
  }

  return sock;
}