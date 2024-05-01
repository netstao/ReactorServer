all:client tcpepoll

client:client.cpp
	g++ -g -o client client.cpp
tcpepoll:tcpepoll.cpp InetAddress.cpp Socket.cpp Epoll.cpp
	g++ -g -o tcpepoll tcpepoll.cpp InetAddress.cpp Socket.cpp Epoll.cpp

clean:
	rm -f client tcpepoll
