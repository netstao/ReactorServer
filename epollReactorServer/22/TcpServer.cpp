#include "TcpServer.h"

TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{

    acceptor_ = new Acceptor(&eloop_, ip, port);
    acceptor_->setnewconnectioncb(std::bind(&TcpServer::newconnection,this,std::placeholders::_1));
    eloop_.setepolltimeoutcallback(std::bind(&TcpServer::epolltimeout,this,std::placeholders::_1));
}

TcpServer::~TcpServer()
{
   delete acceptor_;

   for (auto &cc: conns_) 
   {
    delete cc.second;
   }
}

void TcpServer::start()
{
    eloop_.run();
}

//新连接
 void TcpServer::newconnection(Socket* clientsock) 
 {
     Connection *conn = new Connection(&eloop_, clientsock); //new出来的对象没有释放 以后解决
     conn->setclosecallback(std::bind(&TcpServer::closeconnection,this,std::placeholders::_1));
     conn->seterrorcallback(std::bind(&TcpServer::errorconnection,this,std::placeholders::_1));
     conn->setonmessagecallback(std::bind(&TcpServer::onmessage,this,std::placeholders::_1,std::placeholders::_2));
     conn->setsendcompletecallback(std::bind(&TcpServer::sendcomplete,this,std::placeholders::_1));

     printf ("new client connection (fd=%d,ip=%s,port=%d) ok.\n",conn->fd(),conn->ip().c_str(),conn->port());
     conns_[conn->fd()] = conn;

 }

void TcpServer::closeconnection(Connection *conn) //断开新连接
{
    printf("closeconnection client(eventfd=%d) disconnected.\n",conn->fd());
    // close(conn->fd());            // 关闭客户端的fd。
    conns_.erase(conn->fd());
    delete  conn;
}

void TcpServer::errorconnection(Connection *conn) //错误新连接
{
     printf("errorconnection client(eventfd=%d) error.\n",conn->fd());
    // close(conn->fd());            // 关闭客户端的fd。
    conns_.erase(conn->fd());
    delete  conn;
}

 void TcpServer::onmessage(Connection *conn, std::string message)
 {
    //这里可以解析相应的协议 比如http websocket 等等  然后 应用程序的数据的返回等等等
    // outputBuffer_ = inputBuffer_;
    message="reply:" +message;
    int len = message.size();       //计算报文大小
    std::string tmpbuf((char *)&len,4);  //把报文长度放到头部4个字节
    tmpbuf.append(message);

    // inputBuffer_.clear();   //清空readbuffer 缓冲区
    //send(conn->fd(), tmpbuf.data(), tmpbuf.size(),0);
    conn->send(tmpbuf.data(),tmpbuf.size());
    
 }

 void TcpServer::sendcomplete(Connection *conn)
 {
    printf("send done \n");
 }

 void TcpServer::epolltimeout(EventLoop *eloop)
 {
    printf("TcpServer epoll_wait() timeout. \n");
    //根据业务的需求可以增加其他代码
 }