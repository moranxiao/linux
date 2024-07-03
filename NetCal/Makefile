.PHONY:all
all:server client

server:Server.cc
	g++ $^ -o $@ -std=c++11 -lpthread

client:Client.cc
	g++ $^ -o $@ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -rf server client
