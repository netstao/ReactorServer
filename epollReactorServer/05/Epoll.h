#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <vector>

//epoll 类

class Epoll
{
    private:
        static const int  MaxEvents= 100;  //epoll_wait() 返回事件数组的大小
        int epollfd_ = -1;                 // epoll 句柄 在构造函数创建
        epoll_event events_[MaxEvents];    // 存放poll_wait() 返回时间的数组，在构造函数中分配内存
    public:
        Epoll();                           //构造函数创建epollfd_
        ~Epoll();                          //在析构函数中关闭epollfd_

        void addfd(int fd, uint32_t op);   //把fd和它需要监听的事件添加早红黑树上
        std::vector<epoll_event> loop(int timetout=-1);  //运行epoll_wait() 等待事件的发生，已发生的事件用vector容器返回

};