
all:udp_client udp_server

udp_client:udp_client.cc
	g++ $^ -o $@ -std=c++11 -lpthread -Wdeprecated-declarations
udp_server:udp_server.cc
	g++ $^ -o $@ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f udp_client udp_server