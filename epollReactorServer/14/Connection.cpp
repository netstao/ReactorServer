
#include "Connection.h"

Connection::Connection(EventLoop *eloop, Socket *clientsock):eloop_(eloop),clientsock_(clientsock)
{
   
    clientchannel_ = new Channel(eloop_, clientsock_->fd());
    clientchannel_->setreadcallback(std::bind(&Channel::onmessage, clientchannel_));
    clientchannel_->useet();      //客服端新连接的fd 读事件采用边缘触发
    clientchannel_->enablereading();

}

Connection::~Connection()
{
    delete clientsock_;
    delete clientchannel_;
}



