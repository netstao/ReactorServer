#pragma once

#include <iostream>
#include <string>

// 时间戳
class Timestamp
{
private:
    time_t secsinceepoch_;                       // 整数表示的时间（从1970到现在已逝去的秒数）。

public:
    Timestamp();                                       // 用当前时间初始化对象。
    Timestamp(int64_t secsinceepoch);    // 用一个整数表示的时间初始化对象。

    static Timestamp now();                     // 返回当前时间的Timestamp对象。

    time_t toint() const;                            // 返回整数表示的时间。
    std::string tostring() const;                 // 返回字符串表示的时间，格式：yyyy-mm-dd hh24:mi:ss
};