all:client tcpepoll

client:client.cpp
	g++ -g -o client client.cpp
tcpepoll:tcpepoll.cpp InetAddress.cpp Socket.cpp
	g++ -g -o tcpepoll tcpepoll.cpp InetAddress.cpp Socket.cpp

clean:
	rm -f client tcpepoll
