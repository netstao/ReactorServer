all:client tcpepoll

client:client.cpp
	g++ -g -o client client.cpp
tcpepoll:tcpepoll.cpp InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp Acceptor.cpp Connection.cpp Buffer.cpp
	g++ -g -o tcpepoll tcpepoll.cpp InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp EventLoop.cpp TcpServer.cpp Acceptor.cpp Connection.cpp Buffer.cpp

clean:
	rm -f client tcpepoll
