#include "EventLoop.h"
#include "Channel.h"
#include <unistd.h>
#include <sys/syscall.h>


class Channel;

EventLoop::EventLoop()
          :ep_(new Epoll),wakeupfd_(eventfd(0,EFD_NONBLOCK)),wakechannel_(new Channel(this,wakeupfd_))
{
    wakechannel_->setreadcallback(std::bind(&EventLoop::handlewakeup,this));
    wakechannel_->enablereading();

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