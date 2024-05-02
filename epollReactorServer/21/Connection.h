#pragma once
#include <functional>
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Buffer.h"

class Connection
{
private:
    /* data */
    EventLoop *eloop_ ;   //connection 对应的事件循环 在构造函数中传入.
    Socket *clientsock_;     //客户端连接fd socket
    Channel *clientchannel_; //connection 客服端fd对应的channel 
    Buffer inputBuffer_;   //接收缓冲区
    Buffer outputBuffer_;  //输出发送缓冲区

    std::function<void(Connection*)> closecallback_;  //关闭fd回调， 将调用TcpServer::closeconnection
    std::function<void(Connection*)> errorcallback_;  //关闭fd回调， 将调用TcpServer::errorconnection
    std::function<void(Connection*, std::string)> onmessagecallback_;  //关闭fd回调， 将调用TcpServer::onmessage()


public:
    Connection(EventLoop *eloop, Socket *clientsock);
    ~Connection();
    
    int fd() const;                              // 返回客户端的fd。
    std::string ip() const;                   // 返回客户端的ip。
    uint16_t port() const;                  // 返回客户端的port。

    void closecallback();       //tcp连接断开回调 拱Channel类回调
    void errorcallback();       //tcp连接发生错误 回调 供Channel类回调
    void writecallback();       //tcp连接发生写事件 回调 供Channel类回调

    void setclosecallback( std::function<void(Connection*)> fn);       //tcp连接断开回调 
    void seterrorcallback( std::function<void(Connection*)> fn);       //tcp连接发生错误 
    void setonmessagecallback( std::function<void(Connection*, std::string)> fn);       //消息回调设置
    
    void onmessage();  //处理对端发过来的消息

    void send(const char *data, size_t size);  //发送数据
    
};
