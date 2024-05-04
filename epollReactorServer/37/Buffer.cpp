#include "Buffer.h"

Buffer::Buffer(uint16_t sep):sep_(sep)
{
}

Buffer::~Buffer()
{
}

void Buffer::append(const char *data, size_t size)  //把数据追加到buf_中。
{

    buf_.append(data,size);

}

void Buffer::appendwithsep(const char *data, size_t size)  //添加报文头部
{
    if(sep_==0)
    {
        buf_.append(data,size);
    }
    else if(sep_==1)
    {
        buf_.append((char*)&size,4);   //头部4个字节放入报文的长度数值
        buf_.append(data,size);        //再放入报文内容
    }
    else if(sep_ ==2)
    {

    }
    
} 

size_t Buffer::size()                               //返回buf_大小。
{
    return buf_.size();
}

const char * Buffer::data()                          //返回buf_的首地址。
{
    return buf_.data();
}

void Buffer::clear()      
{
    buf_.clear();

}

void Buffer::erase(size_t pos, size_t nn)
{
    buf_.erase(pos, nn);
}

bool Buffer::pickmessage(std::string &ss)
{
    if(buf_.size()==0)
    {
        return false;
    }

    if(sep_==0)  //没有分隔符
    {
        ss=buf_;
        buf_.clear();
    }
    else if (sep_ ==1)  //4字节报头
    {
        int len;
        memcpy(&len, buf_.data(),4); //从inputBuffer_获取报文头部内容的长度值
        
        if(buf_.size() < len +4) return false;

        std::string message(buf_.data()+4, len); //从inputbuffer中获取一个报文
        ss = buf_.substr(4,len);
        buf_.erase(0, len+4);
    }
    return true;
}
