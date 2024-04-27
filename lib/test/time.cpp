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

    return 0;
}
