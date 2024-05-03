#include "Acceptor.h"
#include "Connection.h"

Acceptor::Acceptor(std::unique_ptr<EventLoop> &eloop, const std::string &ip, const uint16_t port)
        :eloop_(eloop),servsock_(createnonblocking()),acceptchannel_(eloop, servsock_.fd())
{
    // servsock_ = new Socket(createnonblocking());    
    InetAddress servaddr(ip,port);
    servsock_.setreuseaddr(true);
    servsock_.setkeepalive(true);
    servsock_.setreuseport(true);
    servsock_.settcpnodelay(true);
    servsock_.bind(servaddr);
    servsock_.listen();
    // acceptchannel_ = new Channel(eloop_, servsock_.fd()); 

    // acceptchannel_->setreadcallback(std::bind(&Channel::newconnection, acceptchannel_,servsock_));
    acceptchannel_.setreadcallback(std::bind(&Acceptor::newconnection, this));
    acceptchannel_.enablereading();   //让epoll_wait() 监视servchannel读事件
}

Acceptor::~Acceptor()
{
    // delete servsock_;
    // delete acceptchannel_;
}

void Acceptor::setnewconnectioncb(std::function<void(std::unique_ptr<Socket>)> fn)
{
    newconnectioncb_ = fn;
}

void Acceptor::newconnection()  //处理新客服连接请求
{
    InetAddress clientaddr;             // 客户端的地址和协议。
    std::unique_ptr<Socket> clientsock(new Socket(servsock_.accept(clientaddr)));

    clientsock->setipport(clientaddr.ip(), clientaddr.port());

    newconnectioncb_(std::move(clientsock));

}