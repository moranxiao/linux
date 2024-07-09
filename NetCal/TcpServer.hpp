#pragma once
#include "Sock.hpp"
#include <functional>
#include <vector>
#include <pthread.h>
#include <iostream>
#include <sys/types.h>



namespace NetCal
{
    using func_t = std::function<void(int)>;
    class TcpServer;

    class Data
    {
    public:
        Data(int sockfd,TcpServer* server)
        :sockfd_(sockfd),
        server_(server)
        {}
    public:
        int sockfd_;
        TcpServer* server_;
    };

    class TcpServer
    {
    private:
        static void* Execute(void* args)
        {
            Data* pdt = (Data*)args;
            pthread_detach(pthread_self());
            for(auto& f : pdt->server_->funcs_)
            {
                f(pdt->sockfd_);
            }
            return nullptr;
        }
    public:
        TcpServer(uint16_t port,const std::string& ip = "0.0.0.0")
        :listen_sock_(-1)
        {
            listen_sock_ = sock_.Socket();
            sock_.Bind(listen_sock_,ip,port);
            sock_.Listen(listen_sock_);
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
                int sockfd = sock_.Accept(listen_sock_,&client_ip,&client_port);
                if(sockfd < 0)
                    continue;
                std::cout << "client ip:" << client_ip << ",client port:" << client_port << std::endl;
                Data*dt = new Data(sockfd,this);
                pthread_t tid;
                pthread_create(&tid,nullptr,Execute,dt);
            }
        }
        void BindServe(func_t f)
        {
            if(f == nullptr)
                return;
            funcs_.push_back(f);
        }
    private:
        Sock sock_;
        int listen_sock_;
        std::vector<func_t> funcs_;
    };
}