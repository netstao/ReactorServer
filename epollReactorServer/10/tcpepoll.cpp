/*
 * 程序名：tcpepoll.cpp，此程序用于演示采用epoll模型实现网络通讯的服务端。
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
#include <netinet/tcp.h>      // TCP_NODELAY需要包含这个头文件。
#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"
#include "Channel.h"
#include "EventLoop.h"


int main(int argc,char *argv[])
{
    if (argc != 3) 
    { 
        printf("usage: ./tcpepoll ip port\n"); 
        printf("example: ./tcpepoll 192.168.150.128 5085\n\n"); 
        return -1; 
    }

    Socket servsock(createnonblocking());
    InetAddress servaddr(argv[1],atoi(argv[2]));
    servsock.setreuseaddr(true);
    servsock.setkeepalive(true);
    servsock.setreuseport(true);
    servsock.settcpnodelay(true);
    servsock.bind(servaddr);
    servsock.listen();


    /*
    int epollfd=epoll_create(1);        // 创建epoll句柄（红黑树）。

    // 为服务端的listenfd准备读事件。
    epoll_event ev;              // 声明事件的数据结构。
    ev.data.fd=servsock.fd();       // 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回。
    ev.events=EPOLLIN;      // 让epoll监视listenfd的读事件，采用水平触发。

    epoll_ctl(epollfd,EPOLL_CTL_ADD,servsock.fd(),&ev);     // 把需要监视的listenfd和它的事件加入epollfd中。

    epoll_event evs[10];      // 存放epoll_wait()返回事件的数组。
    */

   EventLoop eloop;
//    ep.addfd(servsock.fd(), EPOLLIN); //让Eopll 监听listenfd读事件 默认水平触发
   //ep.addfd(servsock.fd(), EPOLLIN|EPOLLET); //让Eopll 监听listenfd读事件 边缘触发
   //std::vector <epoll_event> evs;


   Channel *servchannel = new Channel(&eloop, servsock.fd());

   servchannel->setreadcallback(std::bind(&Channel::newconnection, servchannel,&servsock));
   servchannel->enablereading();

   eloop.run();

   


   /*
    while (true)        // 事件循环。
    {
       std::vector <Channel *> channel = ep.loop();

       for(auto &ch:channel)
       {

        ch->handleevent();

       }

    }
    */

  return 0;
}