#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <functional>
#include <memory>

class Channel;
class Epoll;

class EventLoop
{
private:
    /* data */
    std::unique_ptr<Epoll> ep_;
    std::function<void(EventLoop*)> epolltimeoutcallback_;  //epoll_wait()超时的回调
public:
    EventLoop();
    ~EventLoop();

    void run();
    Epoll* ep();
    void updateChannel(Channel* ch);    //把channel添加、更新到红黑树上，channel中有fd，也有需要监视的事件
    void removeChannel(Channel* ch);    //从红黑树上删除channel
    void setepolltimeoutcallback(std::function<void(EventLoop*)> fn);
};