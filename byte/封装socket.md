# 封装socket 
```
    /*
    * 程序名：demo7.cpp，此程序用于演示封装socket通讯的客户端
    */
    #include <iostream>
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

    // 向服务端发送报文，成功返回true，失败返回false。
    bool send(const string &buffer)   // buffer不要用const char *
    {
        if (m_clientfd==-1) return false; // 如果socket的状态是未连接，直接返回失败。

        if ((::send(m_clientfd,buffer.data(),buffer.size(),0))<=0) return false;
        
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

    ~ctcpclient(){ close(); }
    };
    
    int main(int argc,char *argv[])
    {
    if (argc!=3)
    {
        cout << "Using:./demo7 服务端的IP 服务端的端口\nExample:./demo7 192.168.101.138 5005\n\n"; 
        return -1;
    }

    ctcpclient tcpclient;
    if (tcpclient.connect(argv[1],atoi(argv[2]))==false)  // 向服务端发起连接请求。
    {
        perror("connect()"); return -1;
    }

    // 第3步：与服务端通讯，客户发送一个请求报文后等待服务端的回复，收到回复后，再发下一个请求报文。
    string buffer;
    for (int ii=0;ii<10;ii++)  // 循环3次，将与服务端进行三次通讯。
    {
        buffer="这是第"+to_string(ii+1)+"个超级女生，编号"+to_string(ii+1)+"。";
        // 向服务端发送请求报文。
        if (tcpclient.send(buffer)==false)
        { 
        perror("send"); break; 
        }
        cout << "发送：" << buffer << endl;

        // 接收服务端的回应报文，如果服务端没有发送回应报文，recv()函数将阻塞等待。
        if (tcpclient.recv(buffer,1024)==false)
        {
        perror("recv()"); break;
        }
        cout << "接收：" << buffer << endl;

        sleep(1);
    }
    }