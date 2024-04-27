//time_t   time.h

//time()

// time_t 结构体

#include <iostream>
#include <time.h>
using namespace std;

int main()
{
    time_t now1 = time(0);
    long now2;
    time(&now2);
    count << "now1=" << now1 << endl;
    count << "now2=" << now2 << endl;

    tm tmnow;
    localtime_r(&now, &tmnow); // 线程安全 time时间戳转出tm时间结构

    string stime = to_string(tmnow.tm_year+1900) + '-' 
                  + to_string(tmnow.tm_mon+1) + '-'
                  + to_string(tmnow.tm_mday) + ' '
                  + to_string(tmnow.tm_hour) + ':'
                  + to_string(tmnow.tm_min) + ':'
                  + to_string(tmnow.tm_sec);
    return 0
}
