#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "Acceptor.h"

class TcpServer
{
private:
    /* data */
    EventLoop  eloop_;
    Acceptor *acceptor_; //一个Tcpserver 只有一个Acceptor对象
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();
    void start(); //启动
};



