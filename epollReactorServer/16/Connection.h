#pragma once
#include <functional>
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"

class Connection
{
private:
    /* data */
    EventLoop *eloop_ ;   //connection 对应的事件循环 在构造函数中传入.
    Socket *clientsock_;     //客户端连接fd socket
    Channel *clientchannel_; //connection 客服端fd对应的channel 

public:
    Connection(EventLoop *eloop, Socket *clientsock);
    ~Connection();
    
    int fd() const;                              // 返回客户端的fd。
    std::string ip() const;                   // 返回客户端的ip。
    uint16_t port() const;                  // 返回客户端的port。
};
