#include "EchoServer.h"


EchoServer::EchoServer(const std::string &ip, const uint16_t port, int subthreadnum, int workthreadnum)
           :tcpserver_(ip,port,subthreadnum),workthreadpool_(workthreadnum, "WORKS")
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

void EchoServer::HandleNewConnection(spConnection conn) //新连接
{
    // printf("HandleNewConnection thread is %ld.\n", syscall(SYS_gettid));
    std::cout<< "new connection come in." << std::endl;
}

void EchoServer::HandleCloseConnection(spConnection conn) //断开新连接
{
    //  printf("HandleCloseConnection thread is %ld.\n", syscall(SYS_gettid));
    std::cout<< "EchoServer connection close." << std::endl;
}

void EchoServer::HandleErrorConnection(spConnection conn) //错误新连接
{
    std::cout<< "EchoServer connection error." << std::endl;
}

void EchoServer::HandleOnMessage(spConnection conn, std::string &message) //处理客户端的请求报文，在TcpServer类中回调此函数。
{
    // printf("HandleOnMessage thread is %ld.\n", syscall(SYS_gettid));
    std::cout<< "EchoServer OnMessage." << std::endl;

    //把业务添加到工作线程池当中
    workthreadpool_.addtask(std::bind(&EchoServer::OnMessage,this,conn,message));
    // OnMessage(conn, message);
}

void EchoServer::OnMessage(spConnection conn, std::string &message)
{
    message="reply:" +message;
    printf("处理完业务后， 将使用connection对象 \n");
    conn->send(message.data(),message.size());  //数据发送出去 可能是野指针 用了智能指针。
}

void EchoServer::HandleSendComplete(spConnection conn)  //数据发送完成后，在TcpServer 回调此函数
{
    std::cout<< "EchoServer Send Done." << std::endl;
}

void EchoServer::HandleEpollTimeOut(EventLoop *eloop)  //epoll_wait()超时， 在TcpServer类中回调此函数
{
    // std::cout<< "EchoServer Epoll TimeOut ." << std::endl;
}
