/*
 * 程序名：tcppoll.cpp，此程序用于演示采用poll模型实现网络通讯的服务端。
 * 作者：吴从周
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>

// 初始化服务端的监听端口。
int initserver(int port);

int main(int argc,char *argv[])
{
   if (argc != 2) { printf("usage: ./tcppoll port\n"); return -1; }

   // 初始化服务端用于监听的socket。
   int listensock = initserver(atoi(argv[1]));
   printf("listensock=%d\n",listensock);

   if (listensock < 0) { printf("initserver() failed.\n"); return -1; }

   pollfd fds[2048];             // fds存放需要监视的socket。

   // 初始化数组，把全部的socket设置为-1，如果数组中的socket的值为-1，那么，poll将忽略它。
   for (int ii=0;ii<2048;ii++)          
      fds[ii].fd=-1;  

   // 打算让poll监视listensock读事件。
   fds[listensock].fd=listensock;
   fds[listensock].events=POLLIN;      // POLLIN表示读事件，POLLOUT表示写事件。
   // fds[listensock].events=POLLIN|POLLOUT;

   int maxfd=listensock;      // fds数组中需要监视的socket的实际大小。

   while (true)      // 事件循环。
   {
      // 调用poll() 等待事件的发生（监视哪些socket发生了事件)。
      int infds=poll(fds,maxfd+1,10000);     // 超时时间为10秒。

      // 如果infds<0，表示调用poll()失败。
      if (infds < 0)
      {
         perror("poll() failed"); break;
      }

      // 如果infds==0，表示poll()超时。
      if (infds == 0)
      {
         printf("poll() timeout.\n"); continue;
      }

      // 如果infds>0，表示有事件发生，infds存放了已发生事件的个数。
      for (int eventfd=0;eventfd<=maxfd;eventfd++)
      {
         if (fds[eventfd].fd<0) continue;                       // 如果fd为负，忽略它。

         if ((fds[eventfd].revents&POLLIN)==0)  continue;  // 如果没有读事件，continue

         // 如果发生事件的是listensock，表示已连接队列中有已经准备好的socket（有新的客户端连上来了）。
         if (eventfd==listensock)
         {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int clientsock = accept(listensock,(struct sockaddr*)&client,&len);
            if (clientsock < 0) { perror("accept() failed"); continue; }

            printf ("accept client(socket=%d) ok.\n",clientsock);

            // 修改fds数组中clientsock位置的元素。
            fds[clientsock].fd=clientsock;
            fds[clientsock].events=POLLIN;

            if (maxfd<clientsock) maxfd=clientsock;   // 更新maxfd的值。
         }
         else
         {
            // 如果是客户端连接的socke有事件，表示有报文发过来了或者连接已断开。

            char buffer[1024]; // 存放从客户端读取的数据。
            memset(buffer,0,sizeof(buffer));
            if (recv(eventfd,buffer,sizeof(buffer),0)<=0)
            {
               // 如果客户端的连接已断开。
               printf("client(eventfd=%d) disconnected.\n",eventfd);

               close(eventfd);           // 关闭客户端的socket。
               fds[eventfd].fd=-1;      // 修改fds数组中clientsock位置的元素，置为-1，poll将忽略该元素。
       
               // 重新计算maxfd的值，注意，只有当eventfd==maxfd时才需要计算。
               if (eventfd == maxfd)
               {
                  for (int ii=maxfd;ii>0;ii--)  // 从后面往前找。
                  {
                     if (fds[ii].fd!=-1)
                     {
                        maxfd = ii; break;
                     }
                  }
               }
            }
            else
            {
               // 如果客户端有报文发过来。
               printf("recv(eventfd=%d):%s\n",eventfd,buffer);

               send(eventfd,buffer,strlen(buffer),0);
            }
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