#pragma once
#include "Epoll.h"

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
};