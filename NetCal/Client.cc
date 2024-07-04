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
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1],&server_addr.sin_addr);
    if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof server_addr) < 0)
    {
        logMessage(ERROR,"connect");
        exit(-1);
    }
    
    run(sockfd);
    
    return 0;
}