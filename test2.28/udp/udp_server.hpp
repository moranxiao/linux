#pragma once

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>

#include "../log.hpp"

class Server{
public:
    Server(uint16_t port,int sock = -1,const std::string& ip = "")
    :_sock(sock), _ip(ip), _port(port)
    {}
    void init()
    {
        //1. 创建socket
        _sock = socket(AF_INET,SOCK_DGRAM,0);
        if(_sock < 0)
        {
            logMessage(FATAL, "server:create socket failed");
            exit(0);
        }
        logMessage(NORMAL, "server:create socket success");
        
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());
        
        //2. bind
        if(bind(_sock,(struct sockaddr*)&local,sizeof local) < 0)
        {
            logMessage(FATAL, "server bind failed");
            exit(0);
        }
        logMessage(NORMAL, "server:bind success");
    }
    void start()
    {
        char buffer[1024];
        while(true)
        {
            memset(buffer,0,sizeof buffer);
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            ssize_t s = recvfrom(_sock,buffer,sizeof(buffer), 0,\
            (sockaddr*)&client, &len);
            if(s > 0)
            {
                //1. 收到客户端的消息并回响
                sendto(_sock,buffer,sizeof(buffer), 0, (sockaddr*)&client, len);
                uint16_t client_port = ntohs(client.sin_port);
                std::string client_ip = inet_ntoa(client.sin_addr);
                std::cout << client_ip << "-" << client_port << ":" << buffer << std::endl;
                
                //2. 执行客服端发来的shell指令
                // FILE* fp = popen(buffer, "r");
                // if(NULL == fp)
                // {
                //     logMessage(WARING, "popen error");
                // }
                // else{
                //     std::string cmd_echo;
                //     char res[256];
                //     while(fgets(res,sizeof(res), fp) != NULL)
                //     {
                //         cmd_echo += res;
                //     }
                //     sendto(_sock,cmd_echo.c_str(),cmd_echo.size(), 0, (sockaddr*)&client, len);
                //     pclose(fp);
                // }
            }
             
        }
    }
    ~Server()
    {
        if(_sock > 0)
            close(_sock);
    }
private:
    int _sock;
    std::string _ip;
    uint16_t _port;
    
};