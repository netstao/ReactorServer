#include "Acceptor.h"
#include "Connection.h"

Acceptor::Acceptor(EventLoop *eloop, const std::string &ip, const uint16_t port):eloop_(eloop)
{
    servsock_ = new Socket(createnonblocking());    
    InetAddress servaddr(ip,port);
    servsock_->setreuseaddr(true);
    servsock_->setkeepalive(true);
    servsock_->setreuseport(true);
    servsock_->settcpnodelay(true);
    servsock_->bind(servaddr);
    servsock_->listen();
    acceptchannel_ = new Channel(eloop_, servsock_->fd()); 

    // acceptchannel_->setreadcallback(std::bind(&Channel::newconnection, acceptchannel_,servsock_));
    acceptchannel_->setreadcallback(std::bind(&Acceptor::newconnection, this));
    acceptchannel_->enablereading();   //让epoll_wait() 监视servchannel读事件
}

Acceptor::~Acceptor()
{
    delete servsock_;
    delete acceptchannel_;
}

void Acceptor::setnewconnectioncb(std::function<void(Socket*)> fn)
{
    newconnectioncb_ = fn;
}

void Acceptor::newconnection()  //处理新客服连接请求
{
    // 注意 clientsock 只能new出来，不能在栈上，否则析构函数会关闭fd。
    // 还有，这里new出来的对象没有释放， 这个问题后面解决
    InetAddress clientaddr;             // 客户端的地址和协议。
    Socket *clientsock = new Socket(servsock_->accept(clientaddr));

    printf ("accept client(fd=%d,ip=%s,port=%d) ok.\n",clientsock->fd(),clientaddr.ip(),clientaddr.port());

    newconnectioncb_(clientsock);

    // Connection *conn = new Connection(eloop_, clientsock); //new出来的对象没有释放 以后解决
}