/* 
 * 程序名：demo11.cpp，此程序用于演示文件传输的客户端。
*/
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

class ctcpclient         // TCP通讯的客户端类。
{
private:
  int m_clientfd;        // 客户端的socket，-1表示未连接或连接已断开；>=0表示有效的socket。
  string m_ip;           // 服务端的IP/域名。
  unsigned short m_port; // 通讯端口。
public:
  ctcpclient():m_clientfd(-1) {}
  
  // 向服务端发起连接请求，成功返回true，失败返回false。
  bool connect(const string &in_ip,const unsigned short in_port)
  {
    if (m_clientfd!=-1) return false; // 如果socket已连接，直接返回失败。

    m_ip=in_ip; m_port=in_port;       // 把服务端的IP和端口保存到成员变量中。

    // 第1步：创建客户端的socket。
    if ( (m_clientfd = socket(AF_INET,SOCK_STREAM,0))==-1) return false;

    // 第2步：向服务器发起连接请求。
    struct sockaddr_in servaddr;               // 用于存放协议、端口和IP地址的结构体。
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;             // ①协议族，固定填AF_INET。
    servaddr.sin_port = htons(m_port);         // ②指定服务端的通信端口。

    struct hostent* h;                         // 用于存放服务端IP地址(大端序)的结构体的指针。
    if ((h=gethostbyname(m_ip.c_str()))==nullptr ) // 把域名/主机名/字符串格式的IP转换成结构体。
    {
      ::close(m_clientfd); m_clientfd=-1; return false;
    }
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length); // ③指定服务端的IP(大端序)。
    
    // 向服务端发起连接清求。
    if (::connect(m_clientfd,(struct sockaddr *)&servaddr,sizeof(servaddr))==-1)  
    {
      ::close(m_clientfd); m_clientfd=-1; return false;
    }

    return true;
  }

  // 向服务端发送报文（字符串），成功返回true，失败返回false。
  bool send(const string &buffer)   // buffer不要用const char *
  {
    if (m_clientfd==-1) return false; // 如果socket的状态是未连接，直接返回失败。

    if ((::send(m_clientfd,buffer.data(),buffer.size(),0))<=0) return false;
    
    return true;
  }

  // 向服务端发送报文（二进制数据），成功返回true，失败返回false。      
  bool send(void *buffer,const size_t size)   
  {
    if (m_clientfd==-1) return false; // 如果socket的状态是未连接，直接返回失败。

    if ((::send(m_clientfd,buffer,size,0))<=0) return false;

    return true;
  }

  // 接收服务端的报文，成功返回true，失败返回false。
  // buffer-存放接收到的报文的内容，maxlen-本次接收报文的最大长度。
  bool recv(string &buffer,const size_t maxlen)
  { // 如果直接操作string对象的内存，必须保证：1)不能越界；2）操作后手动设置数据的大小。
    buffer.clear();         // 清空容器。
    buffer.resize(maxlen);  // 设置容器的大小为maxlen。
    int readn=::recv(m_clientfd,&buffer[0],buffer.size(),0);  // 直接操作buffer的内存。
    if (readn<=0) { buffer.clear(); return false; }
    buffer.resize(readn);   // 重置buffer的实际大小。

    return true;
  }

  // 断开与服务端的连接。
  bool close()
  {
    if (m_clientfd==-1) return false; // 如果socket的状态是未连接，直接返回失败。

    ::close(m_clientfd);
    m_clientfd=-1;
    return true;
  }

  // 向服务端发送文件内容。
  bool sendfile(const string &filename,const size_t filesize)
  {
    // 以二进制的方式打开文件。
    ifstream fin(filename,ios::binary);
    if (fin.is_open() == false) { cout << "打开文件" << filename << "失败。\n";  return false; }

    int  onread=0;        // 每次调用fin.read()时打算读取的字节数。  每次应搬砖头数。
    int  totalbytes=0;    // 从文件中已读取的字节总数。 已搬砖头数。
    char buffer[4096];       // 存放读取数据的buffer。     每次搬七块砖头。

    while (true)
    {
      memset(buffer,0,sizeof(buffer));

      // 计算本次应该读取的字节数，如果剩余的数据超过4096字节，就读4096字节。
      if (filesize-totalbytes>4096) onread=4096;
      else onread=filesize-totalbytes;

      // 从文件中读取数据。
      fin.read(buffer,onread);

      // 把读取到的数据发送给对端。
      if (send(buffer,onread)==false) return false;

      // 计算文件已读取的字节总数，如果文件已读完，跳出循环。
      totalbytes=totalbytes+onread;

      if (totalbytes==filesize) break;
    }

    return true;
  }

 ~ctcpclient(){ close(); }
};
 
int main(int argc,char *argv[])
{
  if (argc!=5)
  {
    cout << "Using:./demo11 服务端的IP 服务端的端口 文件名 文件大小\n";
    cout << "Example:./demo11 192.168.101.138 5005 aaa.txt 2424\n\n";
    return -1;
  }

  ctcpclient tcpclient;
  if (tcpclient.connect(argv[1],atoi(argv[2]))==false)  // 向服务端发起连接请求。
  {
    perror("connect()"); return -1;
  }

  // 以下是发送文件的流程。
  // 1）把待传输文件名和文件的大小告诉服务端。
  // 定义文件信息的结构体。
  struct st_fileinfo{
    char filename[256];  // 文件名。
    int  filesize;       // 文件大小。
  }fileinfo;
  memset(&fileinfo,0,sizeof(fileinfo));
  strcpy(fileinfo.filename,argv[3]);     // 文件名。
  fileinfo.filesize=atoi(argv[4]);       // 文件大小。
  // 把文件信息的结构体发送给服务端。
  if (tcpclient.send(&fileinfo,sizeof(fileinfo))==false) { perror("send"); return -1; }
  cout << "发送文件信息的结构体" << fileinfo.filename << "(" << fileinfo.filesize <<")。"<< endl;

  // 2）等待服务端的确认报文（文件名和文件的大小的确认）。
  string buffer;
  if (tcpclient.recv(buffer,2)==false) { perror("recv()"); return -1; }
  if (buffer!="ok") { cout << "服务端没有回复ok。\n"; return -1; }

  // 3）发送文件内容。
  if (tcpclient.sendfile(fileinfo.filename,fileinfo.filesize)==false)
  { 
    perror("sendfile()"); return -1;
  }
  
  // 4）等待服务端的确认报文（服务端已接收完文件）。
  if (tcpclient.recv(buffer,2)==false) { perror("recv()"); return -1; }
  if (buffer!="ok") { cout << "发送文件内容失败。\n"; return -1; }

  cout << "发送文件内容成功。\n";
}