# ReactorServer
- 基于Epoll，c++11标准实现百万并发Reactor模型, 智能指针，线程，移动语义，原子语义等。
## 基本架构图
![Reactot架构图](/img/Reactor.png)
## 压测
- 30个客户端 每个客户端10万请求一共300万请求
- 本次压测硬件小米笔记本14pro AMD Ryzen 7
- 修改路径[epollReactorServer/37/](epollReactorServer/37/)
- IO线程数的修改[epollReactorServer/server.cpp](epollReactorServer/37/server.cpp)
- ./btest.sh每个客户端连接循环10万次写和读第10万个读完成就直接退出进程
- connectionClose-NewConnection的时间差就是完成报文读写过程花费的时间
## 启动参数
    //参数分别对应ip,port,IO线程数,Work线程数
    server = new EchoServer(argv[1],atoi(argv[2]),32,0);  
    
    ./server 127.0.0.1 9009  #服务端启动 IO线程线程数最好是CPU个数*2

    ./btest.sh  #30个客户端 每个客户端10万请求300万请求

## 压测结果
![压测对比图](/img/b.png)

## 结论 16核心8G 300万请求
- 启动8个IO线程  平均8.6秒内完成300万 平均每秒处理33.33万报文
- 启动16个IO线程 平均2.9秒内完成300万 平均每秒处理103.44万报文 相较8个IO线程提升三倍
- 启动64个IO线程 平均3.8秒内完成300万 平均每秒处理78.94万报文  对比32个IO线程并没有得到明显的提升(每个核的竞争锁更多了) 可以得出结论 IO线程线程数最好是CPU个数*2
- 启动16个IO线程 平均2.3秒内完成300万 平均每秒处理130.43万报文 开启-O3编译优化，得到600毫秒左右的提升，报文每秒提升27万左右,
- "一代又一代的语言想取代C++,代代C++教它们做人" 👍👍👍
## 第一次压测结果  虚拟机WSL 启动8个IO线程 16核心8G
    300万请求 9秒内完成300万 平均每秒处理33.33万报文
    
    2024-05-04 21:45:21 new connection fd=37,ip=127.0.0.1,port=51178, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=38,ip=127.0.0.1,port=51180, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=39,ip=127.0.0.1,port=51182, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=40,ip=127.0.0.1,port=51184, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=41,ip=127.0.0.1,port=51186, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=42,ip=127.0.0.1,port=51188, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=43,ip=127.0.0.1,port=51190, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=44,ip=127.0.0.1,port=51192, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=45,ip=127.0.0.1,port=51194, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=46,ip=127.0.0.1,port=51196, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=47,ip=127.0.0.1,port=51198, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=48,ip=127.0.0.1,port=51200, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=49,ip=127.0.0.1,port=51202, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=50,ip=127.0.0.1,port=51204, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=51,ip=127.0.0.1,port=51206, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=52,ip=127.0.0.1,port=51208, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=53,ip=127.0.0.1,port=51210, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=54,ip=127.0.0.1,port=51212, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=55,ip=127.0.0.1,port=51214, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=56,ip=127.0.0.1,port=51216, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=57,ip=127.0.0.1,port=51218, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=58,ip=127.0.0.1,port=51220, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=59,ip=127.0.0.1,port=51222, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=60,ip=127.0.0.1,port=51224, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=61,ip=127.0.0.1,port=51226, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=62,ip=127.0.0.1,port=51228, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=63,ip=127.0.0.1,port=51230, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=64,ip=127.0.0.1,port=51232, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=65,ip=127.0.0.1,port=51234, thread id is 2548.
    2024-05-04 21:45:21 new connection fd=66,ip=127.0.0.1,port=51236, thread id is 2548.
    2024-05-04 21:45:28 connection close fd=43,ip=127.0.0.1,port=51190 is 2553.
    2024-05-04 21:45:28 connection close fd=51,ip=127.0.0.1,port=51206 is 2553.
    2024-05-04 21:45:28 connection close fd=59,ip=127.0.0.1,port=51222 is 2553.
    2024-05-04 21:45:28 connection close fd=52,ip=127.0.0.1,port=51208 is 2551.
    2024-05-04 21:45:28 connection close fd=44,ip=127.0.0.1,port=51192 is 2551.
    2024-05-04 21:45:28 connection close fd=60,ip=127.0.0.1,port=51224 is 2551.
    2024-05-04 21:45:30 connection close fd=63,ip=127.0.0.1,port=51230 is 2556.
    2024-05-04 21:45:30 connection close fd=56,ip=127.0.0.1,port=51216 is 2549.
    2024-05-04 21:45:30 connection close fd=55,ip=127.0.0.1,port=51214 is 2556.
    2024-05-04 21:45:30 connection close fd=58,ip=127.0.0.1,port=51220 is 2550.
    2024-05-04 21:45:30 connection close fd=47,ip=127.0.0.1,port=51198 is 2556.
    2024-05-04 21:45:30 connection close fd=39,ip=127.0.0.1,port=51182 is 2556.
    2024-05-04 21:45:30 connection close fd=40,ip=127.0.0.1,port=51184 is 2549.
    2024-05-04 21:45:30 connection close fd=48,ip=127.0.0.1,port=51200 is 2549.
    2024-05-04 21:45:30 connection close fd=64,ip=127.0.0.1,port=51232 is 2549.
    2024-05-04 21:45:30 connection close fd=50,ip=127.0.0.1,port=51204 is 2550.
    2024-05-04 21:45:30 connection close fd=66,ip=127.0.0.1,port=51236 is 2550.
    2024-05-04 21:45:30 connection close fd=42,ip=127.0.0.1,port=51188 is 2550.
    2024-05-04 21:45:30 connection close fd=49,ip=127.0.0.1,port=51202 is 2552.
    2024-05-04 21:45:30 connection close fd=65,ip=127.0.0.1,port=51234 is 2552.
    2024-05-04 21:45:30 connection close fd=41,ip=127.0.0.1,port=51186 is 2552.
    2024-05-04 21:45:30 connection close fd=62,ip=127.0.0.1,port=51228 is 2555.
    2024-05-04 21:45:30 connection close fd=54,ip=127.0.0.1,port=51212 is 2555.
    2024-05-04 21:45:30 connection close fd=57,ip=127.0.0.1,port=51218 is 2552.
    2024-05-04 21:45:30 connection close fd=46,ip=127.0.0.1,port=51196 is 2555.
    2024-05-04 21:45:30 connection close fd=37,ip=127.0.0.1,port=51178 is 2554.
    2024-05-04 21:45:30 connection close fd=38,ip=127.0.0.1,port=51180 is 2555.
    2024-05-04 21:45:30 connection close fd=61,ip=127.0.0.1,port=51226 is 2554.
    2024-05-04 21:45:30 connection close fd=53,ip=127.0.0.1,port=51210 is 2554.
    2024-05-04 21:45:30 connection close fd=45,ip=127.0.0.1,port=51194 is 2554.
    `
## 第二次压测结果 虚拟机WSL启动32个IO线程 16核心8G
    300万请求 平均2.9秒内完成300万 平均每秒处理103.44万报文 相较8个IO线程提升三倍
    server = new EchoServer(argv[1],atoi(argv[2]),32,0);  //根目录server.cpp

    2024-05-04 22:13:31 new connection fd=107,ip=127.0.0.1,port=41212, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=108,ip=127.0.0.1,port=41214, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=109,ip=127.0.0.1,port=41216, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=110,ip=127.0.0.1,port=41218, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=111,ip=127.0.0.1,port=41220, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=112,ip=127.0.0.1,port=41222, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=113,ip=127.0.0.1,port=41224, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=114,ip=127.0.0.1,port=41226, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=115,ip=127.0.0.1,port=41228, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=116,ip=127.0.0.1,port=41230, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=117,ip=127.0.0.1,port=41232, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=118,ip=127.0.0.1,port=41234, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=119,ip=127.0.0.1,port=41236, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=120,ip=127.0.0.1,port=41238, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=121,ip=127.0.0.1,port=41240, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=122,ip=127.0.0.1,port=41242, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=123,ip=127.0.0.1,port=41244, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=124,ip=127.0.0.1,port=41246, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=125,ip=127.0.0.1,port=41248, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=126,ip=127.0.0.1,port=41250, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=127,ip=127.0.0.1,port=41252, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=128,ip=127.0.0.1,port=41254, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=129,ip=127.0.0.1,port=41256, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=130,ip=127.0.0.1,port=41258, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=131,ip=127.0.0.1,port=41260, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=132,ip=127.0.0.1,port=41262, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=133,ip=127.0.0.1,port=41264, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=134,ip=127.0.0.1,port=41266, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=135,ip=127.0.0.1,port=41268, thread id is 1502.
    2024-05-04 22:13:31 new connection fd=136,ip=127.0.0.1,port=41270, thread id is 1502.
    2024-05-04 22:13:33 connection close fd=128,ip=127.0.0.1,port=41254 is 1534.
    2024-05-04 22:13:33 connection close fd=114,ip=127.0.0.1,port=41226 is 1527.
    2024-05-04 22:13:33 connection close fd=131,ip=127.0.0.1,port=41260 is 1533.
    2024-05-04 22:13:33 connection close fd=135,ip=127.0.0.1,port=41268 is 1510.
    2024-05-04 22:13:33 connection close fd=122,ip=127.0.0.1,port=41242 is 1519.
    2024-05-04 22:13:33 connection close fd=110,ip=127.0.0.1,port=41218 is 1520.
    2024-05-04 22:13:34 connection close fd=127,ip=127.0.0.1,port=41252 is 1523.
    2024-05-04 22:13:34 connection close fd=124,ip=127.0.0.1,port=41246 is 1532.
    2024-05-04 22:13:34 connection close fd=126,ip=127.0.0.1,port=41250 is 1526.
    2024-05-04 22:13:34 connection close fd=119,ip=127.0.0.1,port=41236 is 1503.
    2024-05-04 22:13:34 connection close fd=107,ip=127.0.0.1,port=41212 is 1508.
    2024-05-04 22:13:34 connection close fd=129,ip=127.0.0.1,port=41256 is 1504.
    2024-05-04 22:13:34 connection close fd=108,ip=127.0.0.1,port=41214 is 1518.
    2024-05-04 22:13:34 connection close fd=109,ip=127.0.0.1,port=41216 is 1514.
    2024-05-04 22:13:34 connection close fd=121,ip=127.0.0.1,port=41240 is 1524.
    2024-05-04 22:13:34 connection close fd=125,ip=127.0.0.1,port=41248 is 1521.
    2024-05-04 22:13:34 connection close fd=113,ip=127.0.0.1,port=41224 is 1525.
    2024-05-04 22:13:34 connection close fd=133,ip=127.0.0.1,port=41264 is 1507.
    2024-05-04 22:13:34 connection close fd=118,ip=127.0.0.1,port=41234 is 1531.
    2024-05-04 22:13:34 connection close fd=111,ip=127.0.0.1,port=41220 is 1512.
    2024-05-04 22:13:34 connection close fd=123,ip=127.0.0.1,port=41244 is 1517.
    2024-05-04 22:13:34 connection close fd=115,ip=127.0.0.1,port=41228 is 1528.
    2024-05-04 22:13:34 connection close fd=132,ip=127.0.0.1,port=41262 is 1506.
    2024-05-04 22:13:34 connection close fd=136,ip=127.0.0.1,port=41270 is 1511.
    2024-05-04 22:13:34 connection close fd=117,ip=127.0.0.1,port=41232 is 1530.
    2024-05-04 22:13:34 connection close fd=120,ip=127.0.0.1,port=41238 is 1509.
    2024-05-04 22:13:34 connection close fd=112,ip=127.0.0.1,port=41222 is 1515.
    2024-05-04 22:13:35 connection close fd=116,ip=127.0.0.1,port=41230 is 1529.
    2024-05-04 22:13:35 connection close fd=134,ip=127.0.0.1,port=41266 is 1522.
    2024-05-04 22:13:35 connection close fd=130,ip=127.0.0.1,port=41258 is 1505.
## 第三次压测 虚拟机WSL启动64个IO线程 16核心8G
    300万请求 对比32个IO线程并没有得到明显的提升 可以得出结论 IO线程线程数最好是CPU个数*2
    server = new EchoServer(argv[1],atoi(argv[2]),32,0);  //根目录server.cpp

    2024-05-04 22:29:05 new connection fd=203,ip=127.0.0.1,port=41298, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=204,ip=127.0.0.1,port=41300, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=205,ip=127.0.0.1,port=41302, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=206,ip=127.0.0.1,port=41304, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=207,ip=127.0.0.1,port=41306, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=208,ip=127.0.0.1,port=41308, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=209,ip=127.0.0.1,port=41310, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=210,ip=127.0.0.1,port=41312, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=211,ip=127.0.0.1,port=41314, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=212,ip=127.0.0.1,port=41316, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=213,ip=127.0.0.1,port=41318, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=214,ip=127.0.0.1,port=41320, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=215,ip=127.0.0.1,port=41322, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=216,ip=127.0.0.1,port=41324, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=217,ip=127.0.0.1,port=41326, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=218,ip=127.0.0.1,port=41328, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=219,ip=127.0.0.1,port=41330, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=220,ip=127.0.0.1,port=41332, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=221,ip=127.0.0.1,port=41334, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=222,ip=127.0.0.1,port=41336, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=223,ip=127.0.0.1,port=41338, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=224,ip=127.0.0.1,port=41340, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=225,ip=127.0.0.1,port=41342, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=226,ip=127.0.0.1,port=41344, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=227,ip=127.0.0.1,port=41346, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=228,ip=127.0.0.1,port=41348, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=229,ip=127.0.0.1,port=41350, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=230,ip=127.0.0.1,port=41352, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=231,ip=127.0.0.1,port=41354, thread id is 21896.
    2024-05-04 22:29:05 new connection fd=232,ip=127.0.0.1,port=41356, thread id is 21896.
    2024-05-04 22:29:08 connection close fd=209,ip=127.0.0.1,port=41310 is 21916.
    2024-05-04 22:29:08 connection close fd=203,ip=127.0.0.1,port=41298 is 21906.
    2024-05-04 22:29:08 connection close fd=214,ip=127.0.0.1,port=41320 is 21922.
    2024-05-04 22:29:08 connection close fd=215,ip=127.0.0.1,port=41322 is 21919.
    2024-05-04 22:29:08 connection close fd=229,ip=127.0.0.1,port=41350 is 21938.
    2024-05-04 22:29:09 connection close fd=206,ip=127.0.0.1,port=41304 is 21913.
    2024-05-04 22:29:09 connection close fd=232,ip=127.0.0.1,port=41356 is 21936.
    2024-05-04 22:29:09 connection close fd=205,ip=127.0.0.1,port=41302 is 21910.
    2024-05-04 22:29:09 connection close fd=224,ip=127.0.0.1,port=41340 is 21935.
    2024-05-04 22:29:09 connection close fd=210,ip=127.0.0.1,port=41312 is 21918.
    2024-05-04 22:29:09 connection close fd=207,ip=127.0.0.1,port=41306 is 21912.
    2024-05-04 22:29:09 connection close fd=227,ip=127.0.0.1,port=41346 is 21937.
    2024-05-04 22:29:09 connection close fd=220,ip=127.0.0.1,port=41332 is 21924.
    2024-05-04 22:29:09 connection close fd=216,ip=127.0.0.1,port=41324 is 21927.
    2024-05-04 22:29:09 connection close fd=204,ip=127.0.0.1,port=41300 is 21911.
    2024-05-04 22:29:09 connection close fd=230,ip=127.0.0.1,port=41352 is 21940.
    2024-05-04 22:29:09 connection close fd=221,ip=127.0.0.1,port=41334 is 21925.
    2024-05-04 22:29:09 connection close fd=226,ip=127.0.0.1,port=41344 is 21923.
    2024-05-04 22:29:09 connection close fd=223,ip=127.0.0.1,port=41338 is 21926.
    2024-05-04 22:29:09 connection close fd=219,ip=127.0.0.1,port=41330 is 21929.
    2024-05-04 22:29:09 connection close fd=208,ip=127.0.0.1,port=41308 is 21909.
    2024-05-04 22:29:09 connection close fd=218,ip=127.0.0.1,port=41328 is 21930.
    2024-05-04 22:29:09 connection close fd=212,ip=127.0.0.1,port=41316 is 21914.
    2024-05-04 22:29:09 connection close fd=217,ip=127.0.0.1,port=41326 is 21928.
    2024-05-04 22:29:09 connection close fd=222,ip=127.0.0.1,port=41336 is 21933.
    2024-05-04 22:29:09 connection close fd=225,ip=127.0.0.1,port=41342 is 21934.
    2024-05-04 22:29:09 connection close fd=228,ip=127.0.0.1,port=41348 is 21915.
    2024-05-04 22:29:09 connection close fd=213,ip=127.0.0.1,port=41318 is 21917.
    2024-05-04 22:29:09 connection close fd=231,ip=127.0.0.1,port=41354 is 21939.
    2024-05-04 22:29:09 connection close fd=211,ip=127.0.0.1,port=41314 is 21920.

## 第四次压测 虚拟机WSL启动32个IO线程 16核8G 开启C++编译 -O3优化
    //300万请求 平均2.3秒内完成300万 得到600毫秒左右的提升 平均每秒处理130.43万报文 报文提升每秒27万左右 

    2024-05-06 11:59:46 new connection fd=107,ip=127.0.0.1,port=38424, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=108,ip=127.0.0.1,port=38426, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=109,ip=127.0.0.1,port=38428, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=110,ip=127.0.0.1,port=38430, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=111,ip=127.0.0.1,port=38432, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=112,ip=127.0.0.1,port=38434, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=113,ip=127.0.0.1,port=38436, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=114,ip=127.0.0.1,port=38438, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=115,ip=127.0.0.1,port=38440, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=116,ip=127.0.0.1,port=38442, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=117,ip=127.0.0.1,port=38444, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=118,ip=127.0.0.1,port=38446, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=119,ip=127.0.0.1,port=38448, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=120,ip=127.0.0.1,port=38452, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=121,ip=127.0.0.1,port=38450, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=122,ip=127.0.0.1,port=38454, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=123,ip=127.0.0.1,port=38456, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=124,ip=127.0.0.1,port=38458, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=125,ip=127.0.0.1,port=38460, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=126,ip=127.0.0.1,port=38462, thread id is 5990.
    2024-05-06 11:59:46 new connection fd=127,ip=127.0.0.1,port=38464, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=128,ip=127.0.0.1,port=38466, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=129,ip=127.0.0.1,port=38468, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=130,ip=127.0.0.1,port=38470, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=131,ip=127.0.0.1,port=38472, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=132,ip=127.0.0.1,port=38474, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=133,ip=127.0.0.1,port=38476, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=134,ip=127.0.0.1,port=38478, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=135,ip=127.0.0.1,port=38480, thread id is 5990.
    2024-05-06 11:59:47 new connection fd=136,ip=127.0.0.1,port=38482, thread id is 5990.
    2024-05-06 11:59:49 connection close fd=116,ip=127.0.0.1,port=38442 is 6013.
    2024-05-06 11:59:49 connection close fd=120,ip=127.0.0.1,port=38452 is 6017.
    2024-05-06 11:59:49 connection close fd=121,ip=127.0.0.1,port=38450 is 6010.
    2024-05-06 11:59:49 connection close fd=129,ip=127.0.0.1,port=38468 is 6022.
    2024-05-06 11:59:49 connection close fd=133,ip=127.0.0.1,port=38476 is 5995.
    2024-05-06 11:59:49 connection close fd=130,ip=127.0.0.1,port=38470 is 5992.
    2024-05-06 11:59:49 connection close fd=111,ip=127.0.0.1,port=38432 is 6004.
    2024-05-06 11:59:49 connection close fd=134,ip=127.0.0.1,port=38478 is 6002.
    2024-05-06 11:59:49 connection close fd=110,ip=127.0.0.1,port=38430 is 6005.
    2024-05-06 11:59:49 connection close fd=119,ip=127.0.0.1,port=38448 is 6015.
    2024-05-06 11:59:49 connection close fd=123,ip=127.0.0.1,port=38456 is 6020.
    2024-05-06 11:59:49 connection close fd=112,ip=127.0.0.1,port=38434 is 6007.
    2024-05-06 11:59:49 connection close fd=125,ip=127.0.0.1,port=38460 is 6016.
    2024-05-06 11:59:49 connection close fd=128,ip=127.0.0.1,port=38466 is 5991.
    2024-05-06 11:59:49 connection close fd=115,ip=127.0.0.1,port=38440 is 6012.
    2024-05-06 11:59:49 connection close fd=113,ip=127.0.0.1,port=38436 is 6008.
    2024-05-06 11:59:49 connection close fd=127,ip=127.0.0.1,port=38464 is 5996.
    2024-05-06 11:59:49 connection close fd=122,ip=127.0.0.1,port=38454 is 6018.
    2024-05-06 11:59:49 connection close fd=118,ip=127.0.0.1,port=38446 is 6009.
    2024-05-06 11:59:50 connection close fd=136,ip=127.0.0.1,port=38482 is 6000.
    2024-05-06 11:59:50 connection close fd=131,ip=127.0.0.1,port=38472 is 5993.
    2024-05-06 11:59:50 connection close fd=132,ip=127.0.0.1,port=38474 is 5994.
    2024-05-06 11:59:50 connection close fd=109,ip=127.0.0.1,port=38428 is 6006.
    2024-05-06 11:59:50 connection close fd=117,ip=127.0.0.1,port=38444 is 6014.
    2024-05-06 11:59:50 connection close fd=124,ip=127.0.0.1,port=38458 is 6021.
    2024-05-06 11:59:50 connection close fd=114,ip=127.0.0.1,port=38438 is 6011.
    2024-05-06 11:59:50 connection close fd=126,ip=127.0.0.1,port=38462 is 6019.
    2024-05-06 11:59:50 connection close fd=135,ip=127.0.0.1,port=38480 is 5997.
    2024-05-06 11:59:50 connection close fd=108,ip=127.0.0.1,port=38426 is 6003.
    2024-05-06 11:59:50 connection close fd=107,ip=127.0.0.1,port=38424 is 6001.
        