#include "EchoServer.h"

EchoServer::EchoServer(const std::string &ip, const uint16_t port):tcpserver_(ip,port)
{
    tcpserver_.setnewconnectioncb(std::bind(&EchoServer::HandleNewConnection,this,std::placeholders::_1));
    tcpserver_.setcloseconnectioncb(std::bind(&EchoServer::HandleCloseConnection,this,std::placeholders::_1));
    tcpserver_.seterrorconnectioncb(std::bind(&EchoServer::HandleErrorConnection,this,std::placeholders::_1));
    tcpserver_.setonmessagecb(std::bind(&EchoServer::HandleOnMessage,this,std::placeholders::_1,std::placeholders::_2));
    tcpserver_.setsendcompletecb(std::bind(&EchoServer::HandleSendComplete,this,std::placeholders::_1));
    tcpserver_.settimeoutcb(std::bind(&EchoServer::HandleEpollTimeOut,this,std::placeholders::_1));
}

EchoServer::~EchoServer()
{
}

void EchoServer::start()
{
    tcpserver_.start();
}

void EchoServer::HandleNewConnection(Connection* conn) //新连接
{
    std::cout<< "new connection come in." << std::endl;
}

void EchoServer::HandleCloseConnection(Connection *conn) //断开新连接
{
    std::cout<< "EchoServer connection close." << std::endl;
}

void EchoServer::HandleErrorConnection(Connection *conn) //错误新连接
{
    std::cout<< "EchoServer connection error." << std::endl;
}

void EchoServer::HandleOnMessage(Connection *conn, std::string &message) //处理客户端的请求报文，在TcpServer类中回调此函数。
{
    std::cout<< "EchoServer OnMessage." << std::endl;
    //这里可以解析相应的协议 比如http websocket 等等  然后 应用程序的数据的返回等等等
    // outputBuffer_ = inputBuffer_;
    /*message="reply:" +message;
    int len = message.size();       //计算报文大小
    std::string tmpbuf((char *)&len,4);  //把报文长度放到头部4个字节
    tmpbuf.append(message);
    */
    // inputBuffer_.clear();   //清空readbuffer 缓冲区
    //send(conn->fd(), tmpbuf.data(), tmpbuf.size(),0);
    conn->send(message.data(),message.size());
}

void EchoServer::HandleSendComplete(Connection *conn)  //数据发送完成后，在TcpServer 回调此函数
{
    std::cout<< "EchoServer Send Done." << std::endl;
}

void EchoServer::HandleEpollTimeOut(EventLoop *eloop)  //epoll_wait()超时， 在TcpServer类中回调此函数
{
    std::cout<< "EchoServer Epoll TimeOut ." << std::endl;
}
