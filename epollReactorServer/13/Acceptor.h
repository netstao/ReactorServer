#pragma once
#include <functional>
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"

class Acceptor
{
private:
    /* data */
    EventLoop *eloop_ ;   //Acceptor 对应的事件循环 在构造函数中传入.
    Socket *servsock_;     //服务端用于监听的socket 在构造函数创建
    Channel *acceptchannel_; //服务端监听fd对应的channel 在acceptor
public:
    Acceptor(EventLoop *eloop, const std::string &ip, const uint16_t port);
    ~Acceptor();
};

