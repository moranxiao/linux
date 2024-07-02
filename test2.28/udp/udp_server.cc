#include "udp_server.hpp"

#include <memory>

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
    std::unique_ptr<Server> server(new Server(port));
    server->init();
    server->start();
    
    return 0;
}