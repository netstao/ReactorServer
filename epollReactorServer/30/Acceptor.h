#pragma once
#include <functional>
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include <memory>

class Acceptor
{
private:
    /* data */
    const std::unique_ptr<EventLoop> &eloop_ ;   //Acceptor 对应的事件循环 在构造函数中传入.
    Socket servsock_;     //服务端用于监听的socket 在构造函数创建
    Channel acceptchannel_; //服务端监听fd对应的channel 在acceptor
    std::function<void(std::unique_ptr<Socket>)> newconnectioncb_; //处理新连接请求的回调函数，指向TcpServer::newconnection()

public:
    Acceptor(std::unique_ptr<EventLoop> &eloop, const std::string &ip, const uint16_t port);
    ~Acceptor();

    void newconnection();  //处理新客服连接请求

    void setnewconnectioncb(std::function<void(std::unique_ptr<Socket>)> fn); //设置新连接fd的回到函数
};

