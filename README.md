# ReactorServer
- 基于Epoll事件，c++实现百万并发Reactor模型。
# 压测 
    ./server 127.0.0.1 9009  #服务端启动
    ./btest.sh  #30个客户端 每个客户端10万请求300万请求
    
## 第一次压测结果  虚拟机WSL 启动8个IO线程 8核心8G
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
## 第二次压测结果 虚拟机WSL启动32个IO线程 8核心8G
    300万请求 平均2.9秒内完成300万 平均每秒处理103.44万报文
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
    