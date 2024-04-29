// demo4.cpp，本程序演示了Linux底层文件的操作-读取文件。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  int fd;    // 定义一个文件描述符/文件句柄。

  fd=open("data.txt",O_RDONLY); // 打开文件。
  if (fd==-1)
  {
    perror("open(data.txt)"); return -1;
  }

  printf("文件描述符fd=%d\n",fd);

  char buffer[1024];
  memset(buffer,0,sizeof(buffer)); // 初始化为0
  if (read(fd,buffer,sizeof(buffer))==-1)    // 从文件中读取数据。
  {
    perror("write()"); return -1;
  }

  printf("%s",buffer);

  close(fd);  // 关闭文件。
}