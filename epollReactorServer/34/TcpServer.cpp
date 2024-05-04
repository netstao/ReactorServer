#include "TcpServer.h"

TcpServer::TcpServer(const std::string &ip, const uint16_t port, int threadnum)
          :threadnum_(threadnum),maineloop_(new EventLoop(true)),acceptor_(maineloop_.get(), ip, port)
          ,Threadepools_(threadnum_,"IO")
{
    // maineloop_=new EventLoop;
    maineloop_->setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1));

    // acceptor_ = new Acceptor(maineloop_, ip, port);
    acceptor_.setnewconnectioncb(std::bind(&TcpServer::newconnection,this,std::placeholders::_1));


    // Threadepools_ = new ThreadPool(threadnum_,"IO");  //创建IO线程池
    //创建从事件循环。
    for(int i=0; i<threadnum_;i++)
    {
        subeloops_.emplace_back(new EventLoop(false,5,10));  //eventLoop 放入从线程循环容器其中 队列尾部
        subeloops_[i]->setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1));
        subeloops_[i]->settimercallback(std::bind(&TcpServer::removeconn,this,std::placeholders::_1));
        Threadepools_.addtask(std::bind(&EventLoop::run,subeloops_[i].get()));
    }

}

TcpServer::~TcpServer()
{
    // delete acceptor_;
    // delete maineloop_;

    // for (auto &cc: conns_) 
    // {
    //     delete cc.second;
    // }

    // for (auto &ss: subeloops_) 
    // {
    //     delete ss; // 释放子线程循环
    // }
    // delete Threadepools_; //释放线程池
}

void TcpServer::start()
{
    maineloop_->run();
}

//新连接
 void TcpServer::newconnection(std::unique_ptr<Socket> clientsock) 
 {
    //  Connection *conn = new Connection(maineloop_, clientsock); //new出来的对象没有释放 以后解决
     spConnection conn(new Connection(subeloops_[clientsock->fd()%threadnum_].get(), std::move(clientsock))); //new出来的对象没有释放 以后解决
     conn->setclosecallback(std::bind(&TcpServer::closeconnection,this,std::placeholders::_1));
     conn->seterrorcallback(std::bind(&TcpServer::errorconnection,this,std::placeholders::_1));
     conn->setonmessagecallback(std::bind(&TcpServer::onmessage,this,std::placeholders::_1,std::placeholders::_2));
     conn->setsendcompletecallback(std::bind(&TcpServer::sendcomplete,this,std::placeholders::_1));

    //  printf ("new client connection (fd=%d,ip=%s,port=%d) ok.\n",conn->fd(),conn->ip().c_str(),conn->port());
     {
        std::lock_guard<std::mutex> gd(mmutex_);
        conns_[conn->fd()] = conn;    //把新连接conn 放入tcpserver conns_ Map容器中
     }
     subeloops_[conn->fd()%threadnum_]->newconnection(conn); //把新连接conn 放入事件循环eloop conns_ Map容器中
     printf(" TcpServer::newconnection thread is %ld\n", syscall(SYS_gettid));
     if(newconnectioncb_) newconnectioncb_(conn);

 }

void TcpServer::closeconnection(spConnection conn) //断开新连接
{
    if(closeconnectioncb_) closeconnectioncb_(conn);
    // printf("closeconnection client(eventfd=%d) disconnected.\n",conn->fd());
    // close(conn->fd());  
     {
        std::lock_guard<std::mutex> gd(mmutex_);          // 关闭客户端的fd。
        conns_.erase(conn->fd());
     }
    // delete  conn;
}

void TcpServer::errorconnection(spConnection conn) //错误新连接
{
    if(errorconnectioncb_) errorconnectioncb_(conn);
    //  printf("errorconnection client(eventfd=%d) error.\n",conn->fd());
    // close(conn->fd());            // 关闭客户端的fd。
     {
        std::lock_guard<std::mutex> gd(mmutex_);
        conns_.erase(conn->fd());
     }
    // delete  conn;
}

void TcpServer::onmessage(spConnection conn, std::string &message)
{
/*
//这里可以解析相应的协议 比如http websocket 等等  然后 应用程序的数据的返回等等等
// outputBuffer_ = inputBuffer_;
message="reply:" +message;
int len = message.size();       //计算报文大小
std::string tmpbuf((char *)&len,4);  //把报文长度放到头部4个字节
tmpbuf.append(message);

// inputBuffer_.clear();   //清空readbuffer 缓冲区
//send(conn->fd(), tmpbuf.data(), tmpbuf.size(),0);
conn->send(tmpbuf.data(),tmpbuf.size());
*/
    if(onmessagecb_) onmessagecb_(conn, message);

}

void TcpServer::sendcomplete(spConnection conn)
{
// printf("send done \n");
    if(sendcompletecb_) sendcompletecb_(conn);
}

void TcpServer::epolltimeout(EventLoop *eloop)
{
// printf("TcpServer epoll_wait() timeout. \n");
    if(timeoutcb_) timeoutcb_(eloop);
//根据业务的需求可以增加其他代码
}

void TcpServer::setnewconnectioncb(std::function<void(spConnection)> fn)
{
    newconnectioncb_=fn;    
}

void TcpServer::setcloseconnectioncb(std::function<void(spConnection)> fn)
{
    closeconnectioncb_=fn;    
}

void TcpServer::seterrorconnectioncb(std::function<void(spConnection)> fn)
{
    errorconnectioncb_=fn;    
}

void TcpServer::setonmessagecb(std::function<void(spConnection, std::string &message)> fn)
{
    onmessagecb_=fn;    
}

void TcpServer::setsendcompletecb(std::function<void(spConnection)> fn)
{
    sendcompletecb_=fn;    
}   

void TcpServer::settimeoutcb(std::function<void(EventLoop*)> fn)
{
    timeoutcb_=fn;    
}

void TcpServer::removeconn(int fd)
{
    printf(" TcpServer::removeconn thread is %ld\n", syscall(SYS_gettid));
     {
        std::lock_guard<std::mutex> gd(mmutex_);
        conns_.erase(fd); //从map 中删除 fd 对应的 conn
     }
}
