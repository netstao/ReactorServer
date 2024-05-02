#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"

class TcpServer
{
private:
    /* data */
    EventLoop  eloop_;
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();
    void start(); //启动
};



