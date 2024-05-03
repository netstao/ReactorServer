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
    std::function<void(Connection*)> newconnectioncb_;  //回调EchoServer::HandleNewConnection().
    std::function<void(Connection*)> closeconnectioncb_;  //回调EchoServer::HandleCloseConnection().
    std::function<void(Connection*)> errorconnectioncb_;  //回调EchoServer::HandleErrorConnection().
    std::function<void(Connection*, std::string &message)> onmessagecb_;  //回调EchoServer::HandleOnMessage().
    std::function<void(Connection*)> sendcompletecb_;  //回调EchoServer::HandleSendComplete().
    std::function<void(EventLoop*)> timeoutcb_;  //回调EchoServer::HandleTimeOut().

    
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();
    void start(); //启动
    void newconnection(Socket* clientsock); //新连接
    void closeconnection(Connection *conn); //断开新连接
    void errorconnection(Connection *conn); //错误新连接
    void onmessage(Connection *conn, std::string message); //处理客户端的请求报文，在Connection类中回调此函数。
    void sendcomplete(Connection *conn);  //数据发送完成后，在Connection 回调此函数
    void epolltimeout(EventLoop *eloop);  //epoll_wait()超时， 在EventLoop类中回调此函数

    void setnewconnectioncb(std::function<void(Connection*)> fn);
    void setcloseconnectioncb(std::function<void(Connection*)> fn);
    void seterrorconnectioncb(std::function<void(Connection*)> fn);
    void setonmessagecb(std::function<void(Connection*, std::string &message)> fn);
    void setsendcompletecb(std::function<void(Connection*)> fn);
    void settimeoutcb(std::function<void(EventLoop*)> fn);
};



