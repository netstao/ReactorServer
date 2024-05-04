#pragma once
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"
#include <map>
#include <memory>

class TcpServer
{
private:
    /* data */
    std::unique_ptr<EventLoop>  maineloop_;    //主线程循环
    std::vector<std::unique_ptr<EventLoop>>  subeloops_;  //存放从事件循环的容器

    Acceptor acceptor_; //一个Tcpserver 只有一个Acceptor对象
    int threadnum_;             //启动多个线程
    ThreadPool Threadepools_;   //线程池对象 存放n个线程

    std::map<int,spConnection> conns_;  //一个tcpserver 有N多个Connection对象存放在map中
    std::function<void(spConnection)> newconnectioncb_;  //回调EchoServer::HandleNewConnection().
    std::function<void(spConnection)> closeconnectioncb_;  //回调EchoServer::HandleCloseConnection().
    std::function<void(spConnection)> errorconnectioncb_;  //回调EchoServer::HandleErrorConnection().
    std::function<void(spConnection, std::string &message)> onmessagecb_;  //回调EchoServer::HandleOnMessage().
    std::function<void(spConnection)> sendcompletecb_;  //回调EchoServer::HandleSendComplete().
    std::function<void(EventLoop*)> timeoutcb_;  //回调EchoServer::HandleTimeOut().

    
public:
    TcpServer(const std::string &ip, const uint16_t port, int threadnum=3);
    ~TcpServer();
    void start(); //启动
    void newconnection(std::unique_ptr<Socket> clientsock); //新连接
    void closeconnection(spConnection conn); //断开新连接
    void errorconnection(spConnection conn); //错误新连接
    void onmessage(spConnection conn, std::string &message); //处理客户端的请求报文，在Connection类中回调此函数。
    void sendcomplete(spConnection conn);  //数据发送完成后，在Connection 回调此函数
    void epolltimeout(EventLoop *eloop);  //epoll_wait()超时， 在EventLoop类中回调此函数

    void setnewconnectioncb(std::function<void(spConnection)> fn);
    void setcloseconnectioncb(std::function<void(spConnection)> fn);
    void seterrorconnectioncb(std::function<void(spConnection)> fn);
    void setonmessagecb(std::function<void(spConnection, std::string &message)> fn);
    void setsendcompletecb(std::function<void(spConnection)> fn);
    void settimeoutcb(std::function<void(EventLoop*)> fn);
};



