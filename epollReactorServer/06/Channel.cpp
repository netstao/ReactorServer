#include "Channel.h"

/*
class Epoll;

class Channel
{
private:
    int  fd_ = -1;   //Channel 拥有fd，Channel 和fd是一对一的关系
    Epoll *ep_ = nullptr;   //Channel 对应的红黑色树，channel与epoll 多对一的关系，一个channel只对应一个Epoll, 一个Epoll对用多个Channel 有服务端监听channel 新的客户端连接的channel 高发可能有N多
    bool inepoll_ = false;   //用于标记Channel 是否已添加到Epoll 树上，如果未添加。调用epoll_ctl()的时候用EPOLL_CTL_ADD 为true时代表已添加 调用epoll_ctl()时用EPOLL_CTL_MOD宏。
    uint32_t events_ = 0;    //fd_ 需要监听的事件，listenfd 和clientfd 需要监听EPOLLIN，clientfd需要监听EPOLLOUT EPOLLIN
    uint32_t revents = 0;  //fd_ 已发生的事件
public:
    Channel(Epoll* ep, int fd);
    ~Channel();

    int fd();       // 返回fd_成员
    void useet();    //边缘触发模式
    void enablereading();  //让epoll_wait()监视fd_的读事件
    void setinepoll();  //把inepoll 成员变量设为true
    void setrevents(uint32_t ev);  //设置revents_成员的值为参数ev
    bool inepoll();  //返回inepoll_ 成员变量的值
    uint32_t events_(); //返回events 成员变量的值
    uint32_t revents_(); //返回revents 成员变量的值
};

*/

Channel::Channel(Epoll* ep, int fd):ep_(ep), fd_(fd)
{
    printf("fd=%d, ep=%p", fd_, &ep);
}

Channel::~Channel()  //析构函数
{
    // 不能销毁 ep_ fd_ 因为不属于Channel类， Channel 这是使用他们而已
}


int Channel::fd()       // 返回fd_成员
{
    return fd_;

}
void Channel::useet()    //边缘触发模式
{
    events_ = events_|EPOLLET;

    printf("events_%d", events_);

}
void Channel::enablereading()  //让epoll_wait()监视fd_的读事件
{
    events_ |= EPOLLIN;
    ep_->updateChannel(this);

}

void Channel::setinepoll()  //把inepoll 成员变量设为true
{
    inepoll_=true;
}

void Channel::setrevents(uint32_t ev)  //设置revents_成员的值为参数ev
{
    revents_ = ev;

}

bool Channel::inepoll()  //返回inepoll_ 成员变量的值
{
    return inepoll_;
}

uint32_t Channel::events() //返回events 成员变量的值
{
    return events_;
}

uint32_t Channel::revents() //返回revents 成员变量的值
{
    return revents_;
}