#include "TcpServer.hpp"
#include "protocol.hpp"
#include "Daemon.hpp"


using namespace NetCal;

void Usage(char* process)
{
    logMessage(ERROR,"Usage:./%s port",process);
}

void Calculator(int sockfd)
{
    while(true)
    {
        std::string buffer;
        if(!Recv(sockfd,&buffer))
            break;
        
        std::string package = DeCode(buffer);
        if(package.empty())
            continue;
        Request req;
        req.DeSerialize(package);
        Response resp = req.Execute();
        std::string send_buffer = resp.Serialize();
        send_buffer = EnCode(send_buffer);

        Send(sockfd,send_buffer);
    }
    close(sockfd);
}
int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        return -1;
    }
    Daemon();
    TcpServer server(atoi(argv[1]));
    server.BindServe(Calculator);
    server.start();
    return 0;
}