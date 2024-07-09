#include <iostream>
#include "Log.hpp"
#include "protocol.hpp"
#include "Sock.hpp"

using namespace NetCal;

void Usage(char* process)
{
    logMessage(ERROR,"Usage:./%s ip port",process);
}

void run(int sockfd)
{
    bool quit = false;
    std::string buffer;
    while(!quit)
    {
        int x,y;
        char op;
        std::cout << "x op y ># ";
        std::cin  >> x >> op >> y;
        Request req(x,y,op);
        std::string s = req.Serialize();
        std::string send_package = EnCode(s);
        Send(sockfd,send_package);
        Response resp;
        while(true)
        {
            if(!Recv(sockfd,&buffer))
            {
                quit == true;
                break;
            }
            std::string package = DeCode(buffer);
            if(package.empty())
            {
                continue;
            }
            resp.DeSerialize(package);
            std::cout << "res:" << resp.result_ << "\tcode:" << resp.code_ << std::endl;
            break;
        }
    }
    close(sockfd);
}

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(-1);
    }
    Sock sock;
    int sockfd = sock.Socket();
    if(!sock.Connect(sockfd,argv[1],atoi(argv[2])))
        return -1;
    run(sockfd);
    
    return 0;
}