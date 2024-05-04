#pragma once
#include <functional>
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Buffer.h"
#include "Timestamp.h"
#include <memory>
#include <atomic>
#include <sys/syscall.h>

class Connection;
class Channel;
class EventLoop;

using spConnection=std::shared_ptr<Connection>;

class Connection:public std::enable_shared_from_this<Connection>
{
private:
    /* data */
    EventLoop *eloop_ ;   //connection 对应的事件循环 在构造函数中传入.
    std::unique_ptr<Socket> clientsock_;     //客户端连接fd socket
    std::unique_ptr<Channel> clientchannel_; //connection 客服端fd对应的channel 
    Buffer inputBuffer_;   //接收缓冲区
    Buffer outputBuffer_;  //输出发送缓冲区
    std::atomic_bool disconnect_;  //客服端是否已断开 断开设为true

    std::function<void(spConnection)> closecallback_;  //关闭fd回调， 将调用TcpServer::closeconnection
    std::function<void(spConnection)> errorcallback_;  //错误fd回调， 将调用TcpServer::errorconnection
    std::function<void(spConnection, std::string&)> onmessagecallback_;  //fd发生消息， 将调用TcpServer::onmessage()
    std::function<void(spConnection)> sendcompletecallback_;  //fd发送数据完成回调， 将调用TcpServer::sendcomplete()

    Timestamp lastatime_;


public:
    Connection(EventLoop *eloop, std::unique_ptr<Socket> clientsock);
    ~Connection();
    
    int fd() const;                              // 返回客户端的fd。
    std::string ip() const;                   // 返回客户端的ip。
    uint16_t port() const;                  // 返回客户端的port。

    void closecallback();       //tcp连接断开回调 拱Channel类回调
    void errorcallback();       //tcp连接发生错误 回调 供Channel类回调
    void writecallback();       //tcp连接发生写事件 回调 供Channel类回调

    void setclosecallback( std::function<void(spConnection)> fn);       //tcp连接断开回调 
    void seterrorcallback( std::function<void(spConnection)> fn);       //tcp连接发生错误 
    void setonmessagecallback( std::function<void(spConnection, std::string&)> fn);       //消息回调设置
    void setsendcompletecallback( std::function<void(spConnection)> fn);       //send完成回调
    
    void onmessage();  //处理对端发过来的消息

    void send(const char *data, size_t size);  //任何线程都调用这个函数发送数据

    void sendinloop(const char *data, size_t size);  //发送数据 如果当前线程时IO线程，直接调用这个此函数，如果是工作线程把此函数传给IO线程

    bool timeout(time_t now, int sec);  //判断连接时否超时 ，空闲太久
    
};
