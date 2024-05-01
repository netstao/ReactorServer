# 三次握手与四次挥手  
    /proc/sys/net/core/somaxconn  /proc/sys/net/ipv4/tcp_max_syn_backlog
    syn=1 seq=x                client->server  listen(fd, min(backlog,somaxconn,tcp_max_syn_backlog))    SYN_RECV  syncookies = 1
    syn=1,ack=1,seq=y,ack=x+1  server->client
    ack=1,seq=x+1,ack=y+1      client->server  min(backlog, somaxconn)  ESTABLISHED  
# 二、四次挥手（握手）
    情景类似：
    1）一端（A）对另一端（B）说：我不会给你发数据了，断开连接吧。 
    2）B回复：ok。（这时候A不能对B发数据了，但是，B仍可以对A发数据）
    3）B发完数据了，对A说：我也不会给你发数据了。（这时候B也不能对A发数据了）。
    4、A回复：ok。
    细节：
    1）主动断开的端在四次挥手后，socket的状态为TIME_WAIT，该状态将持续2MSL（30秒/1分钟/2分钟）。 MSL（Maximum Segment Lifetime）报文在网络上存在的最长时间，超过这个时间报文将被丢弃。
    2）如果是客户端主动断开，TIME_WAIT状态的socket几乎不会造成危害。a）客户端程序的socket很少，服务端程序的socket很多（成千上万）；b）客户端的端口是随机分配的，不存在重用的问题。
    3）如果是服务端主动断开，有两方面的危害：a）socket没有立即释放；b）端口号只能在2MSL后才能重用。
    服务端主动关闭需要等待2MSL()后才能重用
    在服务端程序中，用setsockopt()函数设置socket的属性（一定要放在bind()之前）。
    int opt = 1;   
    setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

