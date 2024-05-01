#include "Socket.h"
/*
//socket 类 封装
class Socket
{
    private:
        const int fd_;  //socket 持有的fd，在构造函数中传进来
    public:
        Socket(int fd);  //构造函数，传入一个已准备好的fd
        ~Socket();      //在析构函数中，close  fd

        int fd() const;   //返回fd成员

        void setreuseaddr(bool on);  //设置SO_REUSEADDR  地址重用选项
        void setreuseport(bool on);  //设置SO_REUSEPORT 端口可以快速重用选项
        void settcpnodelay(bool on);  //设置TCP_NODELAY 选项  nodelay 算法
        void setkeepalive(bool on);  //设置SO_KEEPALIVE 选项 长连接 http

        void bind(const InetAddress& servaddr);  // 服务器绑定的socket地址
        void listen(int backlog=128);            //服务器监听
        int accept(InetAddress& clientAddr);    //服务器获取完成三次握手的， 新来的客户端连接
};
*/

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

int Socket::fd() const
{
    return fd_;
}

void Socket::bind(const InetAddress& servaddr)  // 服务器绑定的socket地址
{
    if (::bind(fd_,servaddr.addr(),sizeof(sockaddr)) < 0 )
    {
        // perror("bind() failed"); close(listenfd); return -1;
         printf("%s:%s:%d bind socket: %d\n", __FILE__, __FUNCTION__,__LINE__,errno);close(fd_);exit(-1);
    }

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

    return clientfd;

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