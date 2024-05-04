
#include "Connection.h"
#include "HttpParser.h"

Connection::Connection(EventLoop *eloop, std::unique_ptr<Socket> clientsock)
           :eloop_(eloop),clientsock_(std::move(clientsock)),disconnect_(false),
           clientchannel_(new Channel(eloop_, clientsock_->fd()))
{
   
    // clientchannel_ = new Channel(eloop_, clientsock_->fd());
    clientchannel_->setreadcallback(std::bind(&Connection::onmessage,this));
    clientchannel_->setclosecallback(std::bind(&Connection::closecallback,this));
    clientchannel_->seterrorcallback(std::bind(&Connection::errorcallback,this));
    clientchannel_->setwritecallback(std::bind(&Connection::writecallback,this));
    clientchannel_->useet();      //客服端新连接的fd 读事件采用边缘触发
    clientchannel_->enablereading();

}

Connection::~Connection()
{
    // delete clientsock_;
    // delete clientchannel_;
    printf("~Connections 对象释放\n");
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
    disconnect_=true;
    clientchannel_->remove();
    closecallback_(shared_from_this());
    // printf("closecallback client(eventfd=%d) disconnected.\n",fd());
    // close(fd());            // 关闭客户端的fd。
}

void Connection::errorcallback()       //tcp连接发生错误 回调 供Channel类回调
{
    disconnect_=true;
    clientchannel_->remove();
    errorcallback_(shared_from_this());
    // printf("errorcallback client(eventfd=%d) error.\n",fd());
    // close(fd());            // 关闭客户端的fd。

}

void Connection::setclosecallback( std::function<void(spConnection)> fn)      //tcp连接断开回调 拱Channel类回调
{
    closecallback_=fn;
}

void Connection::seterrorcallback( std::function<void(spConnection)> fn)       //tcp连接发生错误 回调 供Channel类回调
{
    errorcallback_=fn;
}

void Connection::setonmessagecallback(std::function<void(spConnection, std::string&)> fn)
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

                lastatime_ = Timestamp::now();   //获取时间戳

                // std::cout << "lastatime=" << lastatime_.tostring() << std::endl;

                onmessagecallback_(shared_from_this(), message);  //回调TcpServer类中 onmessage
           
            }
            break;
        } 
        else if (nread == 0)  // 客户端连接已断开。
        {  
            // printf("client(eventfd=%d) disconnected.\n",fd_);
            // close(fd_);            // 关闭客户端的fd。
            // clientchannel_->remove();
            closecallback();   //回调tcpserver close
            break;
        }
    }

}



void Connection::send(const char *data, size_t size)  //发送数据
{
    if(disconnect_==true) { printf("客户端连接已经断开，send()直接返回\n"); return;}


    std::shared_ptr<std::string> message(new std::string(data));
    if (eloop_->isinloopthread())
    {
        sendinloop(message);
    }
    else
    {
        // sendinloop(data, size);
        eloop_->queueinloop(std::bind(&Connection::sendinloop,this,message));
    }
    
}

/*void  Connection::sendinloop(const char *data, size_t size) 
{
    printf("data %s\n",data);
    printf("Connection::sendinloop() thread is %ld.\n", syscall(SYS_gettid));
    outputBuffer_.appendwithhead(data, size);
    std::string message(outputBuffer_.data()+4,size);
    printf("Connection::sendinloop() data is %s, size is %ld.\n",message.c_str(),outputBuffer_.size());
    clientchannel_->enablewriteing();  //注册写事件
}*/

void  Connection::sendinloop(std::shared_ptr<std::string> data) 
{
    // printf("data %s\n",data);
    outputBuffer_.appendwithsep(data->data(), data->size());
    std::string message(outputBuffer_.data()+4,data->size());
    clientchannel_->enablewriteing();  //注册写事件
}

void Connection::writecallback() 
{

    // outputBuffer_.clear();

    // outputBuffer_.append("HTTP/1.1 200 OK\r\n", strlen("HTTP/1.1 200 OK\r\n"));
    // outputBuffer_.append("Host: 127.0.0.1:3000\r\n", strlen("Host: 127.0.0.1:3000\r\n"));
    // outputBuffer_.append("Content-Type: text/html; charset=UTF-8\r\n", strlen("Content-Type: text/html; charset=UTF-8\r\n"));
    // outputBuffer_.append("\r\n", strlen("\r\n"));
    // outputBuffer_.append("HTTP Server", strlen("HTTP Server"));

    printf("data:%s",outputBuffer_.data());
    
    int writen = ::send(fd(), outputBuffer_.data(),outputBuffer_.size(),0); //尝试把outputBuffer中的数据发送出去

    if(writen > 0) outputBuffer_.erase(0,writen); //从outputBuffer_缓冲区中删除已成功发送的字节数

    if(outputBuffer_.size() ==0)
    {
        clientchannel_->disablewriteing();
        sendcompletecallback_(shared_from_this());
    }
}

void Connection::setsendcompletecallback( std::function<void(spConnection)> fn)
{
    sendcompletecallback_ = fn;
}

bool Connection::timeout(time_t now, int sec)   //判断连接是否超时
{
    return (now - lastatime_.toint()) > sec;
}


