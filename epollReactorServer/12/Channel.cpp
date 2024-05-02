#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"
#include "EventLoop.h"



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

    printf("events_%d", events_);

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
        printf("client(eventfd=%d) disconnected.\n",fd_);
        close(fd_);            // 关闭客户端的fd。
    }                                //  普通数据  带外数据
    else if (revents_ & (EPOLLIN|EPOLLPRI))   // 接收缓冲区中有数据可以读。
    {
       /* if (islisten_ == true)   // 如果是listenfd有事件，表示有新的客户端连上来。
        {
            newconnection(servsock);
            ////////////////////////////////////////////////////////////////////////
        }
        else                                        // 如果是客户端连接的fd有事件。
        {
            onmessage(servsock);
        }
        */
       readcallback_();
    }
    else if (revents_ & EPOLLOUT)                  // 有数据需要写，暂时没有代码，以后再说。
    {
    }
    else                                                                   // 其它事件，都视为错误。
    {
        printf("client(eventfd=%d) error.\n",fd_);
        close(fd_);            // 关闭客户端的fd。
    }

} 

void Channel::newconnection(Socket * servsock)  //处理新客服连接请求
{
    // 注意 clientsock 只能new出来，不能在栈上，否则析构函数会关闭fd。
    // 还有，这里new出来的对象没有释放， 这个问题后面解决
    InetAddress clientaddr;             // 客户端的地址和协议。
    Socket *clientsock = new Socket(servsock->accept(clientaddr));

    printf ("accept client(fd=%d,ip=%s,port=%d) ok.\n",clientsock->fd(),clientaddr.ip(),clientaddr.port());

    // 为新客户端连接准备读事件，并添加到epoll中。  这是过程手动方式
    // ev.data.fd=clientsock->fd();
    // ev.events=EPOLLIN|EPOLLET;           // 边缘触发。
    // epoll_ctl(ep.epollfd_,EPOLL_CTL_ADD,clientsock->fd(),&ev);

    //封装类的方式
    // ep.addfd(clientsock->fd(),EPOLLIN|EPOLLET);  //客服端新连接的fd 读事件采用边缘触发

    // 通道方式 加 ev.data.ptr 空指针方式 指向 channel
    printf("clientsock->fd() %d", clientsock->fd());
    Channel *clientchannel = new Channel(eloop_, clientsock->fd());
    clientchannel->setreadcallback(std::bind(&Channel::onmessage, clientchannel));
    clientchannel->useet();      //客服端新连接的fd 读事件采用边缘触发
    clientchannel->enablereading();
}

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
            printf("client(eventfd=%d) disconnected.\n",fd_);
            close(fd_);            // 关闭客户端的fd。
            break;
        }
    }

}

void Channel::setreadcallback(std::function<void()>  fn)
{
    readcallback_ = fn;
}