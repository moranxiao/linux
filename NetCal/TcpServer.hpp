#pragma once
#include "Sock.hpp"
#include <functional>
#include <vector>
#include <pthread.h>
#include <iostream>


using func = std::function<void(int)>;

namespace NetCal
{
    class Data
    {
    public:
        Data(int sockfd,std::vector<func>& funcs)
        :sockfd_(sockfd),
        funcs_(funcs)
        {}
    public:
        int sockfd_;
        std::vector<func>& funcs_;
        pthread_t pid_;
    };

    class TcpServer
    {
    private:
        static void* Run(void* args)
        {
            Data* pdt = (Data*)args;

            pthread_detach(pdt->pid_);
            for(auto& f : pdt->funcs_)
            {
                f(pdt->sockfd_);
            }
            return nullptr;
        }
    public:
        TcpServer(uint16_t port,const std::string& ip = "0.0.0.0")
        :listen_sock_(-1),
        port_(port)
        {
            listen_sock_ = sock_.sock();
            sock_.bindSock(listen_sock_,ip,port);
            sock_.listenSock(listen_sock_);
        }
        ~TcpServer()
        {
            close(listen_sock_);
        }
        void start()
        {
            while(true)
            {
                std::string client_ip;
                uint16_t client_port;
                int sockfd = sock_.acceptSock(listen_sock_,&client_ip,&client_port);
                if(sockfd < 0)
                    continue;
                std::cout << "client ip:" << client_ip << ",client port:" << client_port << std::endl;
                Data*dt = new Data(sockfd,funcs_);
                pthread_create(&dt->pid_,nullptr,Run,dt);
            }
        }
        void pushTask(func f)
        {
            if(f == nullptr)
                return;
            funcs_.push_back(f);
        }
    private:
        Sock sock_;
        int listen_sock_;
        uint16_t port_;
        std::vector<func> funcs_;
    };
}