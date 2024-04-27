#include <string.h>

int main()
{
    char * strerror(int errnum);  //非线程安全
    int strerror(int errnum, char *buf, size_t buflen); //线程安全
    
    return 0;
}