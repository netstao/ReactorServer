#pragma once
#include <string>
#include <cstring>
#include <iostream>

class Buffer
{
private:
    /* data */
    std::string buf_; //用于存放数据
    const uint16_t sep_; //报文分隔符 0无分隔符(固定长度，视频会议)，1四个字节的报头长度，2 \r\n\r\n分隔符

public:
    Buffer(uint16_t sep=1);
    ~Buffer();

    void append(const char *data, size_t size);  //把数据追加到buf_中。
    void appendwithsep(const char *data, size_t size);  //附加报文头部
    void erase(size_t pos, size_t nn);           //从buf_的pos开始删除nn个字节，pos从0开始
    size_t size();                               //返回buf_大小。
    const char *data();                          //返回buf_的首地址。
    void clear();                                //清空buf_
    bool pickmessage(std::string &ss);           //从buf_中拆分一个报文，存放在ss中，没有报文返回false
};


