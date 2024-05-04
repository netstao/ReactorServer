#include "EventLoop.h"



class Channel;

int createtimerfd(int sec=10)
{
    // 把定时器加入epoll。
    int tfd=timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC|TFD_NONBLOCK);   // 创建timerfd。
    struct itimerspec timeout;                                // 定时时间的数据结构。
    memset(&timeout,0,sizeof(struct itimerspec));
    timeout.it_value.tv_sec = sec;                             // 定时时间为5秒。
    timeout.it_value.tv_nsec = 0;
    timerfd_settime(tfd,0,&timeout,0);                  // 开始计时。alarm(5)
    return tfd;
}

EventLoop::EventLoop(bool mainloop, int timetvl, int timeout)
          :ep_(new Epoll),wakeupfd_(eventfd(0,EFD_NONBLOCK)),
          mainloop_(mainloop),
          wakechannel_(new Channel(this,wakeupfd_)),
          timerfd_(createtimerfd(timetvl)),timerchannel_(new Channel(this,timerfd_)),
          timetvl_(timetvl),timeout_(timeout)
{
    wakechannel_->setreadcallback(std::bind(&EventLoop::handlewakeup,this));
    wakechannel_->enablereading();

    timerchannel_->setreadcallback(std::bind(&EventLoop::handletimer,this));
    timerchannel_->enablereading();

}

EventLoop::~EventLoop()
{
    // delete ep_;
}

void EventLoop::run()
{
    threadid_ = syscall(SYS_gettid);

    while (true)        // 事件循环。
    {
       std::vector <Channel *> channels = ep_->loop(10*1000); //等待事件发生 成功发生事件 push到vector容器的尾部
       //为空时是因为执行了ep_->loop中的epoll_wait()超时了 返回了空的channel
       if(channels.size() == 0)
       {
            epolltimeoutcallback_(this);
       }
       else
       {
            for(auto &ch:channels)
            {
                ch->handleevent();
            }
       }
    }
}

// std::unique_ptr<Epoll> EventLoop::ep()
// {
//     return ep_;
// }



void EventLoop::wakeup()
{
    uint64_t val = 1;
    write(wakeupfd_,&val,sizeof(uint64_t));
}

void EventLoop::queueinloop(std::function<void()> fn)
{
    {
        std::lock_guard<std::mutex> gd(mutex_);
        taskqueue_.push(fn);
    }

    //唤醒事件循环
    wakeup();
}

 void EventLoop::handlewakeup()
 {
    printf("handlewakeup thread is %ld\n", syscall(SYS_gettid));
    uint64_t val;
    read(wakeupfd_, &val, sizeof(uint64_t));  //从eventfd中读取数据出来，如果不读取， 在水平触发的模式下eventfd的读事件会一直触发

    std::function<void()> fn;

    std::lock_guard<std::mutex> gd(mutex_);  //给任务队列加锁

    while(taskqueue_.size() >0) 
    {
        fn = std::move(taskqueue_.front());
        taskqueue_.pop();
        fn();
    }
    
 }

bool  EventLoop::isinloopthread()
{
    return threadid_ == syscall(SYS_gettid);
}

void EventLoop::updateChannel(Channel *ch)
{
    ep_->updateChannel(ch);

}

void EventLoop::removeChannel(Channel *ch)
{
    ep_->removeChannel(ch);
}

void EventLoop::setepolltimeoutcallback(std::function<void(EventLoop*)> fn)
{
    epolltimeoutcallback_ = fn;
}

void EventLoop::handletimer()
{
   
    struct itimerspec timeout;                                // 定时时间的数据结构。
    memset(&timeout,0,sizeof(struct itimerspec));
    timeout.it_value.tv_sec = timetvl_;                             // 定时时间为5秒。
    timeout.it_value.tv_nsec = 0;
    timerfd_settime(timerfd_,0,&timeout,0);                  // 开始计时。alarm(5)

     if(mainloop_)
    {
         printf("%ld主事件闹钟时间到了。\n",syscall(SYS_gettid));
    }
    else
    {
        printf("%ld从事件闹钟时间到了, handletimer each conns_ thread is %ld, fd=", syscall(SYS_gettid),syscall(SYS_gettid));
        time_t now = time(0); //获取当前时间

        // if (conns_.size() <=0)
        // { 
        //     printf("conns_.size()=%ld\n",conns_.size());
        //     return;
        // }

        for(auto cc=conns_.begin();cc!=conns_.end();)
        {
            if(cc->first <= 0) 
            { 
                printf("cc->first=0,cc=%d-type=%s-thread=%ld,\n", cc->first, typeid(cc->first).name(),syscall(SYS_gettid));
                continue;
            }
            
            // printf("cc=%d-type=%s-thread=%ld,", cc->first, typeid(cc->first).name(),syscall(SYS_gettid));
            if(cc->second->timeout(now, timeout_))
            {
                printf("where timeout cc=%d-type=%s-thread=%ld,", cc->first, typeid(cc->first).name(),syscall(SYS_gettid));
                timercallback_(cc->first);
                std::lock_guard<std::mutex> gd(mmutex_);
                cc = conns_.erase(cc);  //从map中删除 超时的conn
            } else cc++;
        }
        printf("\n");
    }
   
}

void EventLoop::newconnection(spConnection conn)
{
    printf("EventLoop::newconnectionEventLoop::newconnectionEventLoop::newconnectionEventLoop::newconnection %d\n", conn->fd());
    {
        std::lock_guard<std::mutex> gd(mmutex_);
        conns_[conn->fd()] = conn;
    }
}

void EventLoop::settimercallback(std::function<void(int)> fn)
{
    timercallback_ = fn;
}