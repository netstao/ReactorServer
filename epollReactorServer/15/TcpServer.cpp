
#include "TcpServer.h"

TcpServer::TcpServer(const std::string &ip, const uint16_t port)
{

    acceptor_ = new Acceptor(&eloop_, ip, port);
    acceptor_->setnewconnectioncb(std::bind(&TcpServer::newconnection,this,std::placeholders::_1));
}

TcpServer::~TcpServer()
{
   delete acceptor_;
}

void TcpServer::start()
{
    eloop_.run();
}

//新连接
 void TcpServer::newconnection(Socket* clientsock) 
 {
     Connection *conn = new Connection(&eloop_, clientsock); //new出来的对象没有释放 以后解决
 }