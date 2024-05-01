// 网络通讯的客户端程序。
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>

// 把socket设置成非阻塞。
int setnonblocking(int fd)
{
  int flags;

  // 获取fd的状态。
  if ((flags=fcntl(fd,F_GETFL,0))==-1)
    flags = 0;

  return fcntl(fd,F_SETFL,flags|O_NONBLOCK);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("usage:./client1 ip port\n"); return -1;
  }

  int sockfd;
  struct sockaddr_in servaddr;
  char buf[1024];

  if ((sockfd=socket(AF_INET,SOCK_STREAM,0))<0) { printf("socket() failed.\n"); return -1; }

  setnonblocking(sockfd);     // 把sockfd设置成非阻塞。
  
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(atoi(argv[2]));
  servaddr.sin_addr.s_addr=inet_addr(argv[1]);

  if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)
  {
    if (errno!=EINPROGRESS)
    {
      printf("connect(%s:%s) failed.\n",argv[1],argv[2]); close(sockfd); return -1;
    }
  }

  pollfd fds;
  fds.fd=sockfd;
  fds.events=POLLOUT;
  poll(&fds,1,-1);
  if (fds.revents==POLLOUT)
    printf("connect ok.\n");
  else
    printf("connect failed.\n");
  
return 0;
  // printf("开始时间：%d",time(0));

  for (int ii=0;ii<200000;ii++)
  {
    // 从命令行输入内容。
    memset(buf,0,sizeof(buf));
    printf("please input:"); scanf("%s",buf);
    // strcpy(buf,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbccccccccccccccccddddddddddddd");

    if (send(sockfd,buf,strlen(buf),0) <=0)
    { 
      printf("write() failed.\n"); close(sockfd); return -1;
    }
    
    memset(buf,0,sizeof(buf));
    if (recv(sockfd,buf,sizeof(buf),0) <=0) 
    { 
      printf("read() failed.\n"); close(sockfd); return -1;
    }

    printf("recv:%s\n",buf);
  }

  // printf("结束时间：%d",time(0));
} 