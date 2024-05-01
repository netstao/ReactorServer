# 阻塞io 非阻塞io
    默认阻塞的函数 connect() accept() send() recv()

    在io复用模型中，事件循环不能被阻塞在任何环节，所以应该采用非阻塞io