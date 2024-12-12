#pragma once

#include <string>
#include <sys/select.h>
#include <iostream>

#include "../Sock.hpp"
#include "../Log.h"



class SelectServer{
    static const int MAX_FDS = 1024;
    static const int FD_NONE = -1;
public:
    SelectServer(uint16_t port,const std::string& ip = "0.0.0.0")
    :_port(port),_ip(ip)
    {
        _listenSock = Sock::Socket(true);
        Sock::Bind(_listenSock,_ip,_port);
        Sock::Listen(_listenSock);

        for(int i = 0; i < MAX_FDS; i++)
            _readFds[i] = FD_NONE;
        _readFds[0] = _listenSock;
        LOG(INFO) << "服务器初始化成功\n";
    }
    void Start()
    {
        // struct timeval timeout;
        // timeout.tv_sec = 5;
        // timeout.tv_usec = 0;
        while(1)
        {
            fd_set readFds;
            FD_ZERO(&readFds);
            int maxFds = 0;
            for(int i = 0; i < MAX_FDS; i++)
            {
                if(_readFds[i] != FD_NONE)
                {
                    FD_SET(_readFds[i],&readFds);
                    maxFds = std::max(maxFds,_readFds[i]);
                }   
            }
            // int n = select(maxFds,&readFds,nullptr,nullptr,&timeout);

            int n = select(maxFds+1,&readFds,nullptr,nullptr,nullptr);

            switch(n)
            {
                case 0:
                LOG(DEBUG) << "TIME OUT\n";
                break;
                case -1:
                LOG(WARNING) << "Select Error\n"; 
                break;
                default:
                HandleReadEvent(readFds);
                break;
            }
        }
    }
    ~SelectServer()
    {
        if(_listenSock >= 0) close(_listenSock);
    }
private:
    void HandleReadEvent(const fd_set& readFds)
    {
        for(int i = 0; i < MAX_FDS; i++)
        {
            if(_readFds[i] != FD_NONE && FD_ISSET(_readFds[i],&readFds))
            {
                if(_readFds[i] == _listenSock)
                    Accepter();
                else
                    Reader(i);
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
            if(_readFds[i] == FD_NONE)
            {
                _readFds[i] = sockfd;
                break;
            }
        }
        DebugPrint();
        
    }
    void Reader(int pos)
    {
        int fd = _readFds[pos];
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
            _readFds[pos] = FD_NONE;
        }
        else{
            LOG(ERROR) << "read错误\n";
        }
    }
    void DebugPrint()
    {
        for(int i = 0; i < MAX_FDS; i++)
        {
            if(_readFds[i] != FD_NONE)
                std::cout << _readFds[i] << ' ';
        }
        std::cout << std::endl;
    }
private:
    int _listenSock;
    std::string _ip;
    uint16_t _port;
    int _readFds[MAX_FDS];
};