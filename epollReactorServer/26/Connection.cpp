
#include "Connection.h"

Connection::Connection(EventLoop *eloop, Socket *clientsock):eloop_(eloop),clientsock_(clientsock)
{
   
    clientchannel_ = new Channel(eloop_, clientsock_->fd());
    clientchannel_->setreadcallback(std::bind(&Connection::onmessage,this));
    clientchannel_->setclosecallback(std::bind(&Connection::closecallback,this));
    clientchannel_->seterrorcallback(std::bind(&Connection::errorcallback,this));
    clientchannel_->setwritecallback(std::bind(&Connection::writecallback,this));
    clientchannel_->useet();      //客服端新连接的fd 读事件采用边缘触发
    clientchannel_->enablereading();

}

Connection::~Connection()
{
    delete clientsock_;
    delete clientchannel_;
}

int Connection::fd() const // 返回客户端的fd。
{
    return clientsock_->fd();
}

std::string Connection::ip() const                   // 返回客户端的ip。
{
    return clientsock_->ip();
}

uint16_t Connection::port() const                  // 返回客户端的port。
{
    return clientsock_->port();
}

void Connection::closecallback()       //tcp连接断开回调 拱Channel类回调
{
    closecallback_(this);
    // printf("closecallback client(eventfd=%d) disconnected.\n",fd());
    // close(fd());            // 关闭客户端的fd。
}

void Connection::errorcallback()       //tcp连接发生错误 回调 供Channel类回调
{
    errorcallback_(this);
    // printf("errorcallback client(eventfd=%d) error.\n",fd());
    // close(fd());            // 关闭客户端的fd。

}

void Connection::setclosecallback( std::function<void(Connection*)> fn)      //tcp连接断开回调 拱Channel类回调
{
    closecallback_=fn;
}

void Connection::seterrorcallback( std::function<void(Connection*)> fn)       //tcp连接发生错误 回调 供Channel类回调
{
    errorcallback_=fn;
}

void Connection::setonmessagecallback(std::function<void(Connection*, std::string&)> fn)
{
    onmessagecallback_=fn;
}


void Connection::onmessage()  //处理对端发过来的消息
{

     char buffer[1024];
    while (true)             // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕。
    {    
        bzero(&buffer, sizeof(buffer));
        ssize_t nread = read(fd(), buffer, sizeof(buffer));
        if (nread > 0)      // 成功的读取到了数据。
        {
            // 把接收到的报文内容原封不动的发回去。
            // printf("recv(eventfd=%d):%s\n",fd(),buffer);
            // send(fd(),buffer,strlen(buffer),0);
            inputBuffer_.append(buffer, nread);  //把读取的数据放到接收缓冲区中。
        } 
        else if (nread == -1 && errno == EINTR) // 读取数据的时候被信号中断，继续读取。
        {  
            continue;
        } 
        else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
        {
            

            while (1)
            {
                int len;
                memcpy(&len, inputBuffer_.data(),4); //从inputBuffer_获取报文头部内容的长度值
                
                if(inputBuffer_.size() < len +4) break;

                std::string message(inputBuffer_.data()+4, len); //从inputbuffer中获取一个报文
                inputBuffer_.erase(0, len+4);
                printf("recv(eventfd=%d):%s\n",fd(),message.c_str());

                onmessagecallback_(this, message);  //回调TcpServer类中 onmessage
           
            }
            break;
        } 
        else if (nread == 0)  // 客户端连接已断开。
        {  
            // printf("client(eventfd=%d) disconnected.\n",fd_);
            // close(fd_);            // 关闭客户端的fd。
            closecallback();   //回调tcpserver close
            break;
        }
    }

}

void Connection::send(const char *data, size_t size)  //发送数据
{
    outputBuffer_.appendwithhead(data, size);
    clientchannel_->enablewriteing();  //注册写事件
}

void Connection::writecallback() 
{
    int writen = ::send(fd(), outputBuffer_.data(),outputBuffer_.size(),0); //尝试把outputBuffer中的数据发送出去
    if(writen > 0) outputBuffer_.erase(0,writen); //从outputBuffer_缓冲区中删除已成功发送的字节数

    if(outputBuffer_.size() ==0)
    {
        clientchannel_->disablewriteing();
        sendcompletecallback_(this);
    }
}

void Connection::setsendcompletecallback( std::function<void(Connection*)> fn)
{
    sendcompletecallback_ = fn;
}


