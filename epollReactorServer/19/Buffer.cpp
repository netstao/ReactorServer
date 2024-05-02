#pragma once
#include "Buffer.h"


Buffer::Buffer(/* args */)
{
}

Buffer::~Buffer()
{
}

void Buffer::append(const char *data, size_t size)  //把数据追加到buf_中。
{

    buf_.append(data,size);

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
