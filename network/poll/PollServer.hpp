#pragma once

#include <string>
#include <sys/poll.h>
#include <iostream>

#include "../Sock.hpp"
#include "../Log.h"



class PollServer{
    static const int MAX_FDS = 1024;
    static const int FD_NONE = -1;
public:
    PollServer(uint16_t port,const std::string& ip = "0.0.0.0")
    :_port(port),_ip(ip)
    {
        _listenSock = Sock::Socket(true);
        Sock::Bind(_listenSock,_ip,_port);
        Sock::Listen(_listenSock);

        _fds = new struct pollfd[MAX_FDS];
        for(int i = 0; i < MAX_FDS; i++)
            _fds[i].fd = FD_NONE;
        _fds[0].fd = _listenSock;
        _fds[0].events = POLLIN;
        LOG(INFO) << "服务器初始化成功\n";
    }
    void Start()
    {
        while(1)
        {
            int pollNum = 0;
            for(int i = 0; i < MAX_FDS; i++)
            {
                if(_fds[i].fd != FD_NONE)
                {
                    pollNum = std::max(pollNum,i);
                }
            }
            int n = poll(_fds,pollNum+1,-1);

            switch(n)
            {
                case 0:
                LOG(DEBUG) << "TIME OUT\n";
                break;
                case -1:
                LOG(WARNING) << "Poll Error\n"; 
                break;
                default:
                HandleReadEvent();
                break;
            }
        }
    }
    ~PollServer()
    {
        if(_listenSock >= 0) close(_listenSock);
    }
private:
    void HandleReadEvent()
    {
        for(int i = 0; i < MAX_FDS; i++)
        {
            if(_fds[i].fd != FD_NONE)
            {
                if(_fds[i].revents & POLLIN)
                {
                    if(_fds[i].fd == _listenSock) Accepter();
                    else Reader(i);
                }
            }
        }
    }
    void Accepter()
    {
        int sockfd = Sock::Accept(_listenSock,nullptr,nullptr);
        if(sockfd < 0)
        {
            LOG(WARNING) << "accept error\n";
            return; 
        }
        for(int i = 0; i < MAX_FDS; i++)
        {
            if(_fds[i].fd == FD_NONE)
            {
                _fds[i].fd = sockfd;
                _fds[i].events = POLLIN;
                break;
            }
        }
        DebugPrint();
        
    }
    void Reader(int pos)
    {
        int fd = _fds[pos].fd;
        char buffer[1024];
        int n = read(fd,buffer,sizeof(buffer)-1);
        if(n > 0)
        {
            buffer[n] = 0;
            LOG(INFO) << buffer << std::endl;
        }
        else if(n == 0)
        {
            LOG(INFO) << "客户端关闭[" << fd << "]\n";
            close(fd);
            _fds[pos].fd = FD_NONE;
        }
        else{
            LOG(ERROR) << "read错误\n";
        }
    }
    void DebugPrint()
    {
        for(int i = 0; i < MAX_FDS; i++)
        {
            if(_fds[i].fd != FD_NONE)
                std::cout << _fds[i].fd << ' ';
        }
        std::cout << std::endl;
    }
private:
    int _listenSock;
    std::string _ip;
    uint16_t _port;
    struct pollfd* _fds;
};