#include "Socket.h"


int createnonblocking()
{
    // 创建服务端用于监听的listenfd。
    int listenfd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,IPPROTO_TCP);
    if (listenfd < 0)
    {
        // perror("socket() failed"); exit(-1);
        printf("%s:%s:%d listen socket create error: %d\n", __FILE__, __FUNCTION__,__LINE__,errno);exit(-1);
    }

    return listenfd;
}

Socket::Socket(int fd):fd_(fd)   //构造函数传入一个准备好的fd
{

}

Socket::~Socket()   //析构关闭fd
{
    ::close(fd_);
}

int Socket::fd() const   // 返回fd_成员。
{
    return fd_;
}

std::string Socket::ip() const   // 返回ip_成员。
{
    return ip_;
}

uint16_t Socket::port() const  // 返回port_成员。
{
    return port_;
}

void Socket::bind(const InetAddress& servaddr)  // 服务器绑定的socket地址
{
    if (::bind(fd_,servaddr.addr(),sizeof(sockaddr)) < 0 )
    {
        // perror("bind() failed"); close(listenfd); return -1;
         printf("%s:%s:%d bind socket: %d\n", __FILE__, __FUNCTION__,__LINE__,errno);close(fd_);exit(-1);
    }

    ip_ = servaddr.ip();
    port_ = servaddr.port();

}

void Socket::listen(int backlog)            //服务器监听
{
    if (::listen(fd_, backlog) != 0 )        // 在高并发的网络服务器中，第二个参数要大一些。
    {
        // perror("listen() failed"); close(fd_); exit(-1);
        printf("%s:%s:%d listen socket: %d\n", __FILE__, __FUNCTION__,__LINE__,errno);close(fd_);exit(-1);
    }

}

int Socket::accept(InetAddress& clientAddr)    //服务器获取完成三次握手的， 新来的客户端连接
{
    sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    int clientfd = ::accept4(fd_,(sockaddr*)&peeraddr,&len,SOCK_NONBLOCK);

    clientAddr.setaddr(peeraddr);             // 客户端的地址和协议。

    printf ("accept client(fd=%d,ip=%s,port=%d) ok.\n",clientfd,clientAddr.ip(),clientAddr.port());

    // ip_ = clientAddr.ip();
    // port_ = clientAddr.port();
    setipport(clientAddr.ip(), clientAddr.port());

    return clientfd;

}


void Socket::setipport(const std::string &ip, uint16_t port)
{
    ip_=ip;
    port_=port;
}


void Socket::settcpnodelay(bool on)
{
    int optval= on?1:0;
    ::setsockopt(fd_, IPPROTO_TCP,TCP_NODELAY, &optval, sizeof(optval));
}
void Socket::setreuseaddr(bool on)
{
    int optval= on?1:0;
    ::setsockopt(fd_, SOL_SOCKET,SO_REUSEADDR, &optval, sizeof(optval));
}
void Socket::setreuseport(bool on)
{
    int optval= on?1:0;
    ::setsockopt(fd_, SOL_SOCKET,SO_REUSEPORT, &optval, sizeof(optval));
}
void Socket::setkeepalive(bool on)
{
    int optval= on?1:0;
    ::setsockopt(fd_, SOL_SOCKET,SO_KEEPALIVE, &optval, sizeof(optval));
}