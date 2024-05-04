#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "InetAddress.h"


//创建非阻塞socket
int createnonblocking();

//socket 类 封装
class Socket
{
    private:
        const int fd_;  //socket 持有的fd，在构造函数中传进来
        std::string ip_;  //如果是listenfd， 存放服务端监听的ip，如果是客户端的fd，存放客户端ip
        uint16_t port_;  //如果是listenfd， 存放服务端监听的端口，如果是客户端的fd，存放客户端端口

    public:
        Socket(int fd);  //构造函数，传入一个已准备好的fd
        ~Socket();      //在析构函数中，close  fd

        int fd() const;   //返回fd成员
        std::string ip() const;   //返回ip成员
        uint16_t port() const;   //返回port成员
        void setipport(const std::string &ip, uint16_t port);

        void setreuseaddr(bool on);  //设置SO_REUSEADDR  地址重用选项
        void setreuseport(bool on);  //设置SO_REUSEPORT 端口可以快速重用选项
        void settcpnodelay(bool on);  //设置TCP_NODELAY 选项  nodelay 算法
        void setkeepalive(bool on);  //设置SO_KEEPALIVE 选项 长连接 http

        void bind(const InetAddress& servaddr);  // 服务器绑定的socket地址
        void listen(int backlog=128);            //服务器监听
        int accept(InetAddress& clientAddr);    //服务器获取完成三次握手的， 新来的客户端连接
};