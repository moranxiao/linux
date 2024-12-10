.PHONY:all
all:client server

client:UdpClient.cc
	g++ -o $@ $^ -std=c++11
server:UdpServer.cc
	g++ -o $@ $^ -std=c++11


.PHONY:clean
clean:
	rm -rf client server