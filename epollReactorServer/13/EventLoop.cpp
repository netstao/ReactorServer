#include "EventLoop.h"
#include "Channel.h"

class Channel;

EventLoop::EventLoop():ep_(new Epoll)
{

}

EventLoop::~EventLoop()
{
    delete ep_;
}

void EventLoop::run()
{
      while (true)        // 事件循环。
    {
       std::vector <Channel *> channel = ep_->loop();

       for(auto &ch:channel)
       {

            ch->handleevent();

       }

    }

}

Epoll* EventLoop::ep()
{
    return ep_;
}

void EventLoop::updateChannel(Channel *ch)
{
    ep_->updateChannel(ch);

}