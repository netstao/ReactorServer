#pragma once
#include "Epoll.h"
#include "Channel.h"
#include <functional>
#include <memory>
#include <queue>
#include <map>
#include <mutex>
#include <sys/syscall.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <sys/timerfd.h>      // 定时器需要包含这个头文件。
#include "Connection.h"    

#define MaxTimeOut 5;

class Channel;
class Epoll;
class Connection;

using spConnection=std::shared_ptr<Connection>;

class EventLoop
{
private:
    /* data */
    int timetvl_;                           //闹钟时间间隔 单位秒。
    int timeout_;                          //Connection对象超时时间，秒     
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
    std::map<int,spConnection> conns_;          //存放运行在该事件循环上的全部Connection对象
    std::mutex mmutex_;         //保护conns_互斥锁
    std::function<void(int)> timercallback_;    //删除TcpServer中超时的Connection对象，将被设置为TcpServer::removeconn()
                     


public:
    EventLoop(bool mainloop_, int timetvl=30, int timeout=60);
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
    void newconnection(spConnection conn);   //  把Connection对象保存在Conns_中
    void settimercallback(std::function<void(int)> fn); //设置连接超时回调函数
};
