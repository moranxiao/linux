#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#include "../log.hpp"

void service()
{

}

static void Usage()
{
    logMessage(ERROR,"please input:>./tcp_client x.x.x.x port");
}

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        Usage();
        exit(0);
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock == -1)
    {
        logMessage(FATAL, "client create sock failed");
        exit(0);
    }
    logMessage(NORMAL, "client create sock success");   

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if(connect(sock,(struct sockaddr*)&server,sizeof(sockaddr_in)) == -1)
    {
        logMessage(FATAL, "client connect failed");
        exit(0);
    }  
    logMessage(NORMAL, "client connect success");
    char buffer[1024];
    while(true)
    {
        gets(buffer);
        if(!strcmp(buffer,"quit\n"))
            break;
        write(sock,buffer,strlen(buffer) + 1);
        
        read(sock,buffer,sizeof(buffer));
        std::cout << "server echo:" << buffer << std::endl;
    }
    
    return 0;
}
