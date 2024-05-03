# Reactor 
    Acceptor运行在 Reactor(主进程)中, Connection 运行在从Reactor(线程池中).

    一个从Reactor负责多个Connection, 每个Connection的工作内容包括IO和计算(处理业务)。IO不会阻塞事件循环，但是，计算可能阻塞事件循环。如果计算阻塞了事件循环，那么，在同一Reactor中的全部Connection将会被阻塞。

      负责新来的分配               只负责IO                  负责计算
    ----主Reactor----         ----从Reactor----        ----工作线程----             
    |    Acceptor   |         |    EventLoop1  |      |    workthread1  |
    |               |         |    EventLoop2  |      |    workthread2  |
    |               |         |    EventLoop3  |      |    workthread3  |
    |               |         |    EventLoopN  |      |    workthreadN  |


