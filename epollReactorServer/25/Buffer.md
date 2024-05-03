# Buffer类
    在非阻塞的网络服务程序中，事件循环不会阻塞砸recv和send中，如果数据接收不完整，或者发送缓冲区已经填满，都不能等待，所以Buffer是必须的。
    在Reactor模型中，每个Connection对象拥有InputBuffer和SendBuffer.

# TcpConnection
## inputBuffer
    TCP 是一个无边界字节流协议，接收方必须要处理"收到的数据尚不构成一条完整的消息" 和 "一次收到两条消息的数据" 等情况.一个常见的场景是， 发送方 send（）了两条1kb的消息(2kB)，接收方收到的数据情况可能是:
        - 一次性收到2kB数据；
        - 分两次收到，一次600B，第二次1400B;
        - 分两次  一次 1KB 一次1kB；
        - 分三次 一次600B 一次800B 一次600B；
        长度为N字节的消息分块到达的可能性有2的(N-1)次方种。
        网络库在处理 socket 可读事件时，必须一次性把socket里的数据读完。否则会反复触发POLLIN事件，造成busy-loop. 面对不完整数据的情况，必须先放到 inputBuffer 里，等构成一条完整的数据，在通知程序的业务逻辑，所以在网络编程中，必须配置inputBuffer
## outputBuffer
    Tcpconnection 必须要有output buffer 考虑一个常见场景：程序想通过TCP连接发送100kB的数据，但是在write调用中，操作系统只接受了80kB(受TCP advertised window的控制，细节减[TCPv1]), 你肯定不想在原地等待，因为不知道会等多久(取决于对方什么时候接收户数据，然后滑动TCP窗口)。程序应该尽快交出控制权，返回 eventloop。 在这种情况下，剩余的20kB数据怎么办?
    对于应用程序而言，它只管生成数据，它不应该关心到底数据是一次性发完还是分成几次发送，这些应该由网络库来操心，程序只要调用TcpConnection::send()就行了，网络库会负责到底。网络库应该接管这剩余的20KB数据，把它保存在该TCP connnection outputBuffer中，然后注册POLLOUT事件，一旦socket变得可写就立刻发送数据。当然，这里第二次write()也不一定能完全写入20kB，如果还有剩余，网络库应该继续关注POLLOUT事件；如果写完了20KB，网络库应该停止关注POLLOUT，以免造成busy loop.
    