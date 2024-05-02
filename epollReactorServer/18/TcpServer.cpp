#include "TcpServer.h"

TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{

    acceptor_ = new Acceptor(&eloop_, ip, port);
    acceptor_->setnewconnectioncb(std::bind(&TcpServer::newconnection,this,std::placeholders::_1));
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