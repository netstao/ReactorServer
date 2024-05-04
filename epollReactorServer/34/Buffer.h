#pragma once
#include <string>
#include <iostream>

class Buffer
{
private:
    /* data */
    std:: string buf_; //用于存放数据

public:
    Buffer(/* args */);
    ~Buffer();

    void append(const char *data, size_t size);  //把数据追加到buf_中。
    void appendwithhead(const char *data, size_t size);  //附加报文头部
    void erase(size_t pos, size_t nn);           //从buf_的pos开始删除nn个字节，pos从0开始
    size_t size();                               //返回buf_大小。
    const char *data();                          //返回buf_的首地址。
    void clear();                                //清空buf_
};


