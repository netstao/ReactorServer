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
