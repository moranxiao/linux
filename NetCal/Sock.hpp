#pragma once
#include "Log.hpp"
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>



class Sock{
private:
    const static int gbacklog = 20;
public:
    int Socket()
    {
        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(sockfd < 0)
        {
            logMessage(ERROR,"获取套接字错误");
            exit(2);
        }
        return sockfd;
    }
    void Bind(int sockfd,const std::string& ip,uint16_t port)
    {
        struct sockaddr_in addr; 
        memset(&addr,0,sizeof addr);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET,ip.c_str(),&addr.sin_addr.s_addr);       
        if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)) == -1)
        {
            logMessage(ERROR,"bind");
            exit(3);
        }
    }
    void Listen(int sockfd)
    {  
        if(listen(sockfd,gbacklog) < 0)
        {
            logMessage(ERROR,"listen");
            exit(4);
        }
    }
    int Accept(int sockfd,std::string* ip,uint16_t* port)
    {
        struct sockaddr_in addr;
        memset(&addr,0,sizeof addr);
        socklen_t len = sizeof(addr);
        int servicesock = accept(sockfd,(struct sockaddr*)&addr,&len);
        if(servicesock < 0)
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
        return servicesock;      
    }
    bool Connect(int sockfd,const std::string& ip,uint16_t port)
    {
        struct sockaddr_in server_addr;
        memset(&server_addr,0,sizeof server_addr);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
        if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof server_addr) < 0)
        {
            logMessage(ERROR,"connect");
            return false;
        }
        return true;
    }
};
