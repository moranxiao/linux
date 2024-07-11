#include "HttpServer.hpp"
#include "Log.hpp"
#include "Usage.hpp"
#include "Sock.hpp"
#include <memory>


void HanderHttpRequest(int sockfd)
{
    char buffer[10240];

    ssize_t sz = recv(sockfd,buffer,sizeof(buffer)-1,0);
    if(sz <= 0) return;
    buffer[sz] = 0;
    
    std::cout << buffer << "----------------\n" << std::endl;
}

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::unique_ptr<HttpServer> server(new HttpServer(atoi(argv[1]),HanderHttpRequest));
    server->Start();
    return 0;
}