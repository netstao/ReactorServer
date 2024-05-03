#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"
#include "EventLoop.h"
// #include "Connection.h"



Channel::Channel(EventLoop* eloop, int fd):eloop_(eloop), fd_(fd)
{
    // printf("fd=%d, ep=%p", fd_, &ep);
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

    // printf("events_%d", events_);

}
void Channel::enablereading()  //让epoll_wait()监视fd_的读事件
{
    events_ |= EPOLLIN;
    //ep_->updateChannel(this);
    eloop_->updateChannel(this);


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

void Channel::handleevent()
{

    if (revents_ & EPOLLRDHUP)                     // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
    {
        printf("EPOLLRDHUP\n");
        // printf("client(eventfd=%d) disconnected.\n",fd_);
        // close(fd_);            // 关闭客户端的fd。
        closecallback_();
    }                                //  普通数据  带外数据
    else if (revents_ & (EPOLLIN|EPOLLPRI))   // 接收缓冲区中有数据可以读。
    {
        printf("EPOLLIN\n");
        readcallback_();
    }
    else if (revents_ & EPOLLOUT)                  // 有数据需要写，暂时没有代码，以后再说。
    {
        printf("EPOLLOUT\n");
        writecallback_();
        
    }
    else                                                                   // 其它事件，都视为错误。
    {
        // printf("client(eventfd=%d) error.\n",fd_);
        // close(fd_);            // 关闭客户端的fd。
         errorcallback_();
    }

} 

/*
void Channel::onmessage()  //处理对端发过来的消息
{

     char buffer[1024];
    while (true)             // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕。
    {    
        bzero(&buffer, sizeof(buffer));
        ssize_t nread = read(fd_, buffer, sizeof(buffer));
        if (nread > 0)      // 成功的读取到了数据。
        {
            // 把接收到的报文内容原封不动的发回去。
            printf("recv(eventfd=%d):%s\n",fd_,buffer);
            send(fd_,buffer,strlen(buffer),0);
        } 
        else if (nread == -1 && errno == EINTR) // 读取数据的时候被信号中断，继续读取。
        {  
            continue;
        } 
        else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
        {
            break;
        } 
        else if (nread == 0)  // 客户端连接已断开。
        {  
            // printf("client(eventfd=%d) disconnected.\n",fd_);
            // close(fd_);            // 关闭客户端的fd。
            closecallback_();
            break;
        }
    }

}*/

void Channel::setreadcallback(std::function<void()>  fn)
{
    readcallback_ = fn;
}

void Channel::setclosecallback(std::function<void()>  fn) //客户端fd关闭回调 回调Connecttion::closecallback()
{
    closecallback_ = fn;
}

void Channel::seterrorcallback(std::function<void()>  fn) //客户端fd发生错误回调函数 回调Connecttion::errorcallback()
{
     errorcallback_ = fn;
}

void Channel::disablereading()  //让epoll_wait()取消fd_的读事件
{
    events_ &= ~EPOLLIN;
    eloop_->updateChannel(this);
}

void Channel::enablewriteing()  //让epoll_wait()注册fd_的写事件
{
    events_ |= EPOLLOUT;
    eloop_->updateChannel(this);

}

void Channel::disablewriteing()  //让epoll_wait()取消fd_的写事件
{
    events_ &= ~EPOLLOUT;
    eloop_->updateChannel(this);
}

void Channel::setwritecallback(std::function<void()>  fn)  //设置写 事件回调 EPOLLOUT
{
    writecallback_ = fn;
}

