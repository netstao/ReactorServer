
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




