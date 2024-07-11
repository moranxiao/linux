#pragma once

#include "Sock.hpp"
#include <functional>
#include <iostream>
#include <signal.h>

using func_t = std::function<void(int)>;


class HttpServer
{
public:
    HttpServer(uint16_t port,func_t func,const std::string& ip = "0.0.0.0")
    {
        listensock_ = sock_.Socket();
        sock_.Bind(listensock_,ip,port);
        sock_.Listen(listensock_); 
        func_ = func;
        signal(SIGCHLD,SIG_IGN);   
    }
    void Start()
    {
        while(true)
        {
            std::string client_ip;
            uint16_t client_port;
            int sockfd = sock_.Accept(listensock_,&client_ip,&client_port);
            if(sockfd < 0) continue;
            
            std::cout << "client ip:" << client_ip << " " << "client port:" << client_port << std::endl;
            
            if(fork() == 0)
            {
                close(listensock_);
                func_(sockfd);
                close(sockfd);
                exit(0);
            }
            close(sockfd);
        }
    }
    ~HttpServer()
    {
        if(listensock_ >= 0) close(listensock_);
    }
private:
    Sock sock_;
    int listensock_;
    func_t func_;
};