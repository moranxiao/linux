#pragma once
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <functional>

#include "../Sock.hpp"
#include "../Log.h"

class TcpServer;

struct ThreadData{
    int sockfd;
    pthread_t tid;
    TcpServer* server;
};

class TcpServer{
public:
    TcpServer(uint16_t port,const std::string& ip = "0.0.0.0")
    :_port(port),_ip(ip)
    {
        _listenSock = Sock::Socket(true);
        Sock::Bind(_listenSock,_ip,_port);
        Sock::Listen(_listenSock);
        LOG(INFO) << "服务器初始化成功\n";
    }
    void Start()
    {
        while(1)
        {
            int sock = Sock::Accept(_listenSock,nullptr,nullptr);
            if(sock < 0)
            {
                LOG(ERROR) << "接受链接失败\n";
                continue;
            }
            //1.多进程
            //1.1:子进程执行服务,父进程等待子进程
            // pid_t pid = fork();
            // if(pid < 0)
            // {
            //     LOG(FATAL) << "创建子进程失败\n";
            //     exit(-1);
            // }
            // if(pid > 0){
            //     waitpid(pid,nullptr,0);
            // }
            // else{
            //     HandleEvent();
            //     exit(0);
            // }

            //1.2:子进程执行服务,父进程不关心子进程
            // pid_t pid = fork();
            // if(pid == 0){
            //     //父进程退出，子进程变为孤儿进程，被OS收养
            //     if(fork() > 0) exit(0);
            //     else
            //     {
            //         HandleEvent();
            //         exit(0);
            //     }
            // }
            // else{
            //     waitpid(pid,nullptr,0);
            // }

            //2多线程
            //2.1:创建线程完成服务
            ThreadData td;
            td.sockfd = sock;
            td.server = this;
            if(pthread_create(&td.tid,nullptr,Run,&td))
            {
                LOG(FATAL) << "创建线程失败\n";
            }
            pthread_join(td.tid,nullptr);

            //2.2:线程池
            
            //TODO
        }
    }


    ~TcpServer()
    {
        if(_listenSock >= 0) close(_listenSock);
    }
private:
    static void* Run(void* args)
    {
        ThreadData* ptd = (ThreadData*)args;
        LOG(INFO) << "线程[" << ptd->tid << "]:正在运行";
        ptd->server->HandleEvent();       
        return nullptr; 
    }
    void HandleEvent()
    {
        LOG(DEBUG) << "hehe\n";
    }
    void Recver()
    {
        
    }
    void Send()
    {

    }
private:
    std::string _ip;
    uint16_t _port;
    int _listenSock;  
};