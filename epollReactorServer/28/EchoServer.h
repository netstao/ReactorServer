#pragma once
#include "TcpServer.h"
#include "EventLoop.h"
#include "Connection.h"
#include "ThreadPool.h"

//回显服务器

class EchoServer
{
private:
    /* data */
    TcpServer tcpserver_;
    ThreadPool workthreadpool_;

public:
    EchoServer(const std::string &ip, const uint16_t port, int subthreadnum=3, int workthreadnum=5);
    ~EchoServer();


    void start();
    void HandleNewConnection(Connection* conn); //新连接
    void HandleCloseConnection(Connection *conn); //断开新连接
    void HandleErrorConnection(Connection *conn); //错误新连接
    void HandleOnMessage(Connection *conn, std::string &message); //处理客户端的请求报文，在TcpServer类中回调此函数。
    void HandleSendComplete(Connection *conn);  //数据发送完成后，在TcpServer 回调此函数
    void HandleEpollTimeOut(EventLoop *eloop);  //epoll_wait()超时， 在TcpServer类中回调此函数

    void OnMessage(Connection *conn, std::string &message); //工作线程池中 工作的线程 添加到线程池的任务队列中
};


