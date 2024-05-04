#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <functional>
#include <memory>
#include <queue>
#include <mutex>
#include <sys/syscall.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <sys/timerfd.h>      // 定时器需要包含这个头文件。

class Channel;
class Epoll;

class EventLoop
{
private:
    /* data */
    std::unique_ptr<Epoll> ep_;
    std::function<void(EventLoop*)> epolltimeoutcallback_;  //epoll_wait()超时的回调
    pid_t threadid_;
    std::queue<std::function<void()>> taskqueue_;  //事件循环线程被eventfd唤醒后执行的任务队列
    std::mutex mutex_;                              //任务队列同步的互斥锁
    int wakeupfd_;                                 //唤醒事件循环线程的eventfd 也就是io线程
    std::unique_ptr<Channel> wakechannel_;   //   eventfd channel
    int timerfd_;                                //定时器的fd
    std::unique_ptr<Channel> timerchannel_;     //定时器Channel
    bool mainloop_;                             //true-是主事件循环，false-是从事件循环

public:
    EventLoop(bool mainloop_);
    ~EventLoop();

    void run();
    Epoll* ep();
    void updateChannel(Channel* ch);    //把channel添加、更新到红黑树上，channel中有fd，也有需要监视的事件
    void removeChannel(Channel* ch);    //从红黑树上删除channel
    void setepolltimeoutcallback(std::function<void(EventLoop*)> fn);

    bool isinloopthread();  //判断当前线程是否为事件循环线程

    void queueinloop(std::function<void()> fn);  //把发送任务添加到任务队列
    void wakeup();  //唤醒事件循环
    void handlewakeup();  //事件循环被eventfd唤醒后执行的函数

    void handletimer();   //  闹钟响应执行函数
};