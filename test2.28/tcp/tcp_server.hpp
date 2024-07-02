#pragma once

#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <signal.h>

#include "../log.hpp"
#include "../thread.hpp"

const static int gbacklog = 20;

class Data{
public:
    int _sock;
    uint16_t _port;
    std::string _ip;
};

void service(int servicesock,const std::string ip,uint16_t port)
{
    char buffer[1024];
    while(true)
    {
        ssize_t s = read(servicesock, buffer,sizeof(buffer));
        if(s > 0)
        {
            buffer[s-1] = '\0';
            std::cout << ip << "|" << port << ":" << buffer << std::endl;
            write(servicesock, buffer, s);
        }
        else if(s == 0)
        {
            logMessage(NORMAL, "%s:%d shutdown, me too!", ip.c_str(), port);
            break;
        }
        else
        {
            logMessage(ERROR, "read socket error, %d:%s", errno, strerror(errno));
            break;
        }
    }
    close(servicesock);
    

}


class Server{
private:
    static void* thread_routine(void* args)
    {
        pthread_detach(pthread_self());
        Data* pdata = (Data*)args;
        service(pdata->_sock,pdata->_ip,pdata->_port);
        delete pdata;
        return nullptr;
    }
public:
    Server(uint16_t port,const std::string ip = "",int sock = -1)
    :_listensock(sock),_port(port),_ip(ip)
    {}
    void init()
    {
        _listensock = socket(AF_INET, SOCK_STREAM, 0);
        if(_listensock < 0)
        {
            logMessage(FATAL, "server:create socket failed");
            exit(0);
        }
        logMessage(NORMAL, "server:create socket success");
        
        struct sockaddr_in server;
        server.sin_addr.s_addr = _ip.empty() ? INADDR_ANY : inet_addr(_ip.c_str());
        server.sin_port = htons(_port);
        server.sin_family = AF_INET;
        if(bind(_listensock,(struct sockaddr*)&server,sizeof(server)) < 0)
        {
            logMessage(FATAL, "server bind failed");
            exit(0);
        }
        logMessage(NORMAL, "server:bind success");
        
        if(listen(_listensock, gbacklog) < 0)
        {
            logMessage(FATAL, "listen error");
            exit(0);
        }
        logMessage(NORMAL, "listen success");
        

    }
    void start()
    {
        // signal(SIGCHLD,SIG_DFL);

        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        while(true)
        {
            int sock = accept(_listensock,(struct sockaddr*)&client, &len);
            if(sock < 0)
            {
                logMessage(WARING, "connect failed");
                continue;
            }

            uint16_t client_port = ntohs(client.sin_port);
            std::string client_ip = inet_ntoa(client.sin_addr);
            logMessage(NORMAL, "connect success:%s|%d", client_ip.c_str(),client_port);

            //1.1 多进程版 （修改默认进程结束信号的处理方法）
            // int pid = fork();
            // if(pid == 0)
            // {
            //     close(_listensock);
            //     service(sock,client_ip,client_port);
            //     exit(0); 
            // }
            //1.2 多进程版 （创建子进程后，子进程再创建子进程，然后自己推出，处理任务的进程变为孤儿进程）
            // int pid = fork();
            // if(pid == 0)
            // {
            //     if(fork() > 0) exit(0);
            //     close(_listensock);
            //     service(sock,client_ip,client_port);
            //     exit(0); 
            // }
            //1.3 多进程版（主进程进行非阻塞等待子进程）
            //太简单了

            //2.1 多线程版
            Data* pdata = new Data;
            pdata->_ip = client_ip;
            pdata->_port = client_port;
            pdata->_sock = sock;
            pthread_t tid;
            pthread_create(&tid,nullptr,thread_routine,pdata);


        }
    }
    ~Server()
    {
        if(_listensock >= 0)
            close(_listensock);
    }
private:
    uint16_t _port;
    std::string _ip;
    int _listensock;
};