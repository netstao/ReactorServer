#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"
#include <map>

class TcpServer
{
private:
    /* data */
    EventLoop  eloop_;
    Acceptor *acceptor_; //一个Tcpserver 只有一个Acceptor对象
    std::map<int,Connection *> conns_;  //一个tcpserver 有N多个Connection对象存放在map中

    
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();
    void start(); //启动
    void newconnection(Socket* clientsock); //新连接
    void closeconnection(Connection *conn); //断开新连接
    void errorconnection(Connection *conn); //错误新连接
};



