# 异步通知事件循环
    通知线程的方法: 条件变量、信号量、socket、管道、eventfd。
    事件循环阻塞在epoll_wait()函数，条件变量，信号量，有自己的等待函数，不适合用于通知事件循环。
    socket、管道、eventfd都是fd，可加入epoll，如果要通知事件循环，往socket，管道、eventfd中写入数据即可。