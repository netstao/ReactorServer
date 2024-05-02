
#include "Connection.h"

Connection::Connection(EventLoop *eloop, Socket *clientsock):eloop_(eloop),clientsock_(clientsock)
{
   
    clientchannel_ = new Channel(eloop_, clientsock_->fd());
    clientchannel_->setreadcallback(std::bind(&Channel::onmessage,clientchannel_));
    clientchannel_->setclosecallback(std::bind(&Connection::closecallback,this));
    clientchannel_->seterrorcallback(std::bind(&Connection::errorcallback,this));
    clientchannel_->useet();      //客服端新连接的fd 读事件采用边缘触发
    clientchannel_->enablereading();

}

Connection::~Connection()
{
    delete clientsock_;
    delete clientchannel_;
}

int Connection::fd() const // 返回客户端的fd。
{
    return clientsock_->fd();
}

std::string Connection::ip() const                   // 返回客户端的ip。
{
    return clientsock_->ip();
}

uint16_t Connection::port() const                  // 返回客户端的port。
{
    return clientsock_->port();
}

void Connection::closecallback()       //tcp连接断开回调 拱Channel类回调
{
    closecallback_(this);
    // printf("closecallback client(eventfd=%d) disconnected.\n",fd());
    // close(fd());            // 关闭客户端的fd。
}

void Connection::errorcallback()       //tcp连接发生错误 回调 供Channel类回调
{
    errorcallback_(this);
    // printf("errorcallback client(eventfd=%d) error.\n",fd());
    // close(fd());            // 关闭客户端的fd。

}

void Connection::setclosecallback( std::function<void(Connection*)> fn)      //tcp连接断开回调 拱Channel类回调
{
    closecallback_=fn;
}

void Connection::seterrorcallback( std::function<void(Connection*)> fn)       //tcp连接发生错误 回调 供Channel类回调
{
    errorcallback_=fn;
}


void Connection::onmessage()  //处理对端发过来的消息
{

     char buffer[1024];
    while (true)             // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕。
    {    
        bzero(&buffer, sizeof(buffer));
        ssize_t nread = read(fd(), buffer, sizeof(buffer));
        if (nread > 0)      // 成功的读取到了数据。
        {
            // 把接收到的报文内容原封不动的发回去。
            printf("recv(eventfd=%d):%s\n",fd(),buffer);
            send(fd(),buffer,strlen(buffer),0);
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
            cc();
            break;
        }
    }

}



