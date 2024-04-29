// demo3.cpp，本程序演示了Linux底层文件的操作-创建文件并写入数据。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

int main()
{
  int fd;    // 定义一个文件描述符/文件句柄。

  // 打开文件，注意，如果创建后的文件没有权限，可以手工授权chmod 777 data.txt。
  fd=open("data.txt",O_CREAT|O_RDWR|O_TRUNC);
  if (fd==-1)
  {
    perror("open(data.txt)"); return -1;
  }

  printf("文件描述符fd=%d\n",fd);

  char buffer[1024];
  strcpy(buffer,"我是一只傻傻鸟。\n");

  if (write(fd,buffer,strlen(buffer))==-1)    // 把数据写入文件。
  {
    perror("write()"); return -1;
  }

  close(fd);  // 关闭文件。
}