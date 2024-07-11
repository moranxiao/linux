.PHONY:all
all:CalServer CalClient

CalServer:CalServer.cc
	g++ $^ -o $@ -std=c++11 -lpthread -ljsoncpp

CalClient:CalClient.cc
	g++ $^ -o $@ -std=c++11 -lpthread -ljsoncpp

.PHONY:clean
clean:
	rm -rf CalServer CalClient
