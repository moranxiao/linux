#pragma once

#include <cassert>
#include <string>

#include "Epoll.hpp"
#include "../Sock.hpp"
#include "../Log.h"


class EpollServer{
    const static int timeout = 15000;
public:
    EpollServer(uint16_t port,const std::string& ip = "0.0.0.0")
    :_port(port),_ip(ip),_timeout(timeout)
    {
        _listenSock = Sock::Socket(true);
        Sock::Bind(_listenSock,_ip,_port);
        Sock::Listen(_listenSock);
        
        _revts = new struct epoll_event[1024];
        _maxEvents = 1024;
        _epoll.AddEventToEpoll(_listenSock,EPOLLIN);
        LOG(INFO) << "server initialize success!" << '\n';
    }
    void LoopOnce()
    {
        int n = _epoll.EpollWait(_revts,_maxEvents,_timeout);
        switch(n)
        {
            case 0:
            LOG(INFO) << "time out\n";
            break;
            case -1:
            LOG(ERROR) << "epoll error";
            break;
            default:
            HandleEvent(n);
        }
    }
    void HandleEvent(int n)
    {
        assert(n > 0);
        for(int i = 0; i < n; i++)
        {
            if(_revts[i].events & EPOLLIN)
            {
                if(_revts[i].data.fd == _listenSock)
                {
                    Accept();
                }
                else{
                    Recver(_revts[i].data.fd);
                }
            }
        }
    }
    void Accept()
    {
        int sock = Sock::Accept(_listenSock,nullptr,nullptr);
        if(sock < 0)
        {
            LOG(WARNING) << "Accept error\n"; 
            return;
        }
        if(!_epoll.AddEventToEpoll(sock,EPOLLIN))
            LOG(WARNING) << "AddEventToEpoll error\n"; 
        LOG(INFO) << "Accept Sock success:" << sock << '\n';
    }
    void Recver(int sock)
    {
        char buffer[10240];
        int n = read(sock,buffer,sizeof(buffer)-1);
        if(n > 0)
        {
            buffer[n] = 0;
            LOG(INFO) << "sock[" << sock << "]:" << buffer;
        }
        else if(n==0)
        {
            if(!_epoll.DeleteSockFromEpoll(sock))
                LOG(WARNING) << "DeleteSockFromEpoll error\n"; 
            
            close(sock);
            LOG(INFO) << "close sock:" << sock << "\n"; 
        }
        else{
            LOG(WARNING) << "read sock error:" << sock << "\n"; 
        }
    }
    void Start()
    {
        while(1)
        {
            LoopOnce();
        }
    }
    
    ~EpollServer()
    {
        if(_listenSock >= 0) close(_listenSock);
    }
private:
    std::string _ip;
    uint16_t _port;
    int _listenSock;
    Epoll _epoll;
    struct epoll_event* _revts;
    int _maxEvents;
    int _timeout;
};