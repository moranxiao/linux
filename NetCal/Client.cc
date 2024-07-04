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
    while(true)
    {
        int x,y;
        char op;
        std::cout << "x op y" << std::endl;
        std::cin  >> x >> op >> y;
        Request req(x,y,op);
        std::string str = req.Serialize();
        if(write(sockfd,str.c_str(),str.length()) <= 0)
            break;
        char buffer[1024];
        int sz = read(sockfd,buffer,sizeof(buffer));
        if(sz <= 0)
            break;
        Response resp;
        resp.DeSerialize(buffer);
        std::cout << resp.Serialize() << std::endl;
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