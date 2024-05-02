
#include "Epoll.h"
/*
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
*/

Epoll::Epoll()                           //构造函数创建epollfd_
{
    epollfd_=epoll_create(1);
    if(epollfd_==-1)
    {
        printf("epoll_create() failed(%d)\n",errno); 
        perror("epoll_create() failed perror");exit(-1);
    }
}
Epoll::~Epoll()
{
    close(epollfd_);
}

void Epoll::updateChannel(Channel *ch)    //把channel添加、更新到红黑树上，channel中有fd，也有需要监视的事件
{
    epoll_event ev;
    ev.data.ptr=ch;
    ev.events = ch->events();  //需要监听的事件集合 EPOLLIN  EPOLLOUT

    if (ch->inepoll())
    {
        if(epoll_ctl(epollfd_, EPOLL_CTL_MOD, ch->fd(),&ev) == -1)
        {
            perror("epoll_ctl  EPOLL_CTL_MOD failed.\n"); exit(-1);
        }
    }
    else   //不在channel 树上
    {
        if(epoll_ctl(epollfd_, EPOLL_CTL_ADD, ch->fd(),&ev) == -1)
        {
            perror("epoll_ctl EPOLL_CTL_ADD failed.\n"); exit(-1);
        }
        ch->setinepoll();  //添加成功 channel  调用ch->setinepoll()设置为true
    }
}

/*
void Epoll::addfd(int fd, uint32_t op)   //把fd和它需要监听的事件添加早红黑树上
{
    epoll_event ev;              // 声明事件的数据结构。
    ev.data.fd=fd;       // 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回。
    ev.events=op;      // 让epoll监视listenfd的读事件，采用水平触发。

    printf("epollfd_%d, %d \n", epollfd_, fd);

    if(epoll_ctl(epollfd_,EPOLL_CTL_ADD,fd,&ev) == -1)    // 把需要监视的listenfd和它的事件加入epollfd中。
    {
        printf("epoll_ctl() failed(%d)\n",errno);
         perror("epoll_ctl() failed perror");exit(-1);
    }
}
*/

/*
std::vector<epoll_event> Epoll::loop(int timetout)  //运行epoll_wait() 等待事件的发生，已发生的事件用vector容器返回
{
    std::vector<epoll_event> evs;   //存放epoll_wait()返回事件

    bzero(events_,sizeof(events_));

    int infds=epoll_wait(epollfd_,events_,MaxEvents,timetout);       // 等待监视的fd有事件发生。

    // 返回失败。
    if (infds < 0)
    {
        perror("epoll_wait() failed");exit(-1);
    }

    // 超时。
    if (infds == 0)
    {
        printf("epoll_wait() timeout.\n"); return evs;
    }

    // 如果infds>0，表示有事件发生的fd的数量。
    for (int ii=0;ii<infds;ii++)       // 遍历epoll返回的数组evs。
    {
        evs.push_back(events_[ii]);

    }

    return evs;
} 
*/

std::vector<Channel *> Epoll::loop(int timetout)  //运行epoll_wait() 等待事件的发生，已发生的事件用vector容器返回
{
    std::vector<Channel *> channel;   //存放epoll_wait()返回事件

    bzero(events_,sizeof(events_));

    int infds=epoll_wait(epollfd_,events_,MaxEvents,timetout);       // 等待监视的fd有事件发生。

    // 返回失败。
    if (infds < 0)
    {
        perror("epoll_wait() failed");exit(-1);
    }

    // 超时。
    if (infds == 0)
    {
        printf("epoll_wait() timeout.\n"); return channel;
    }

    // 如果infds>0，表示有事件发生的fd的数量。
    for (int ii=0;ii<infds;ii++)       // 遍历epoll返回的数组evs。
    {
        // evs.push_back(events_[ii]);
        Channel *ch = (Channel *)events_[ii].data.ptr;
        ch->setrevents(events_[ii].events);
        channel.push_back(ch);

    }

    return channel;
} 