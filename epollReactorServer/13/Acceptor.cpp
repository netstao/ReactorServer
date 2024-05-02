#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *eloop, const std::string &ip, const uint16_t port):eloop_(eloop)
{
    servsock_ = new Socket(createnonblocking());    
    InetAddress servaddr(ip,port);
    servsock_->setreuseaddr(true);
    servsock_->setkeepalive(true);
    servsock_->setreuseport(true);
    servsock_->settcpnodelay(true);
    servsock_->bind(servaddr);
    servsock_->listen();
    acceptchannel_ = new Channel(eloop_, servsock_->fd()); 

    acceptchannel_->setreadcallback(std::bind(&Channel::newconnection, acceptchannel_,servsock_));
    acceptchannel_->enablereading();   //让epoll_wait() 监视servchannel读事件
}

Acceptor::~Acceptor()
{
    delete servsock_;
    delete acceptchannel_;
}
