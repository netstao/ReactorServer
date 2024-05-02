
#include "TcpServer.h"

TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{
    Socket *servsock = new Socket(createnonblocking());    //分配了内存没有释放，以后再说
    InetAddress servaddr(ip,port);
    servsock->setreuseaddr(true);
    servsock->setkeepalive(true);
    servsock->setreuseport(true);
    servsock->settcpnodelay(true);
    servsock->bind(servaddr);
    servsock->listen();
    Channel *servchannel = new Channel(&eloop_, servsock->fd()); //分配了内存没有释放，以后再说

    servchannel->setreadcallback(std::bind(&Channel::newconnection, servchannel,servsock));
    servchannel->enablereading();   //让epoll_wait() 监视servchannel读事件
}

TcpServer::~TcpServer()
{
   
}

void TcpServer::start()
{
    eloop_.run();
}