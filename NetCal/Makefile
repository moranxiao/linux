.PHONY:all
all:CalServer CalClient

CalServer:CalServer.cc
	g++ $^ -o $@ -std=c++11 -lpthread

CalClient:CalClient.cc
	g++ $^ -o $@ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -rf CalServer CalClient
