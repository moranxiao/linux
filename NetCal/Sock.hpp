#include "Log.hpp"
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


#define BACKLOG 10

class Sock{
public:
    int sock()
    {
        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
        {
            logMessage(ERROR,"获取套接字错误");
            exit(-1);
        }
    }
    void bindSock(int sockfd,const std::string& ip,uint16_t port)
    {
        struct sockaddr_in addr; 
        memset(&addr,0,sizeof addr);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET,ip.c_str(),&addr.sin_addr.s_addr);       
        if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)) == -1)
        {
            logMessage(ERROR,"bind");
            close(sockfd);
            exit(-1);
        }
    }
    void listenSock(int sockfd)
    {  
        if(listen(sockfd,BACKLOG) < 0)
        {
            logMessage(ERROR,"listen");
            close(sockfd);
            exit(-1);
        }
    }
    int acceptSock(int sockfd,std::string* ip,uint16_t* port)
    {
        struct sockaddr_in addr;
        memset(&addr,0,sizeof addr);
        socklen_t len;
        int ret = accept(sockfd,(struct sockaddr*)&addr,&len);
        if(ret < 0)
        {
            logMessage(WARING,"accept");
            return -1;
        }
        if(ip != nullptr)
        {
            *ip = inet_ntoa(addr.sin_addr);
        }
        if(port != nullptr)
        {
            *port = ntohs(addr.sin_port);
        }
        return ret;      
    }
};