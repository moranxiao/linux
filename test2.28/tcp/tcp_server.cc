#include <memory>

#include "tcp_server.hpp"


static void Usage()
{
    logMessage(ERROR,"please input:>./udp_server port");
}

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        Usage();
        exit(0);
    }
    uint16_t port = atoi(argv[1]);
    std::unique_ptr<Server> ser(new Server(port));
    ser->init();
    ser->start();
    return 0;
}