#pragma once
#include "Epoll.h"
#include "Channel.h"

class Channel;
class Epoll;

class EventLoop
{
private:
    /* data */
    Epoll *ep_;
public:
    EventLoop();
    ~EventLoop();

    void run();
    Epoll* ep();
    void updateChannel(Channel* ch);    //把channel添加、更新到红黑树上，channel中有fd，也有需要监视的事件
};