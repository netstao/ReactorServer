# ReactorServer
- 基于Epoll事件，c++实现百万并发Reactor模型。
# 压测 
    虚拟机WSL 启动8个IO线程 8核心8G
    
    ./server 127.0.0.1 9009  #服务端启动
    
    ./btest.sh  #30个客户端 每个客户端10万请求300万请求
    
## 压测结果  
    -- 300万请求 9秒内完成300万 平均每秒处理33.33万报文
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
        
