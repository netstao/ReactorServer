#include "EventLoop.h"
#include "Channel.h"
#include <unistd.h>
#include <sys/syscall.h>


class Channel;

EventLoop::EventLoop():ep_(new Epoll)
{

}

EventLoop::~EventLoop()
{
    // delete ep_;
}

void EventLoop::run()
{
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