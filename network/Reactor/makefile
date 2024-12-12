all:client server

server:Server.cc
	g++ -o $@ $^ -std=c++11 -ljsoncpp
client:Client.cc
	g++ -o $@ $^ -std=c++11 -ljsoncpp

.PHONY:clean
clean:
	rm -rf server client