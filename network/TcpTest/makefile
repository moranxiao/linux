.PHONY:all
all:client server

client:TcpClient.cc
	g++ -o $@ $^ -std=c++11
server:TcpServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -rf client server