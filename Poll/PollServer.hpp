#ifndef POLLSERVER
#define POLLSERVER

#include "Sock.hpp"
#include <poll.h>
#include <iostream>

#define NUM 1024
#define FD_NONE -1

class PollServer{
private:
    void HandlerEvent()
    {
        for(int i = 0; i < NUM; i++)
        {
            if(_fds[i].fd == _listensock && _fds[i].revents & POLLIN)
                Accept();
            else if(_fds[i].fd != FD_NONE && _fds[i].revents & POLLIN)
                Receive(i);
        }
    }
    void Accept()
    {
        std::string client_ip;
        uint16_t client_port;
        int sockfd = _sock.Accept(_listensock,&client_ip,&client_port);
        if(sockfd < 0)
            logMessage(WARING,"accept error");
        else{
            int i = 0;
            for(; i < NUM; i++)
            {
                if(_fds[i].fd == FD_NONE)
                    break;
            }
            if(i < NUM)
            {
                _fds[i].fd = sockfd;
                _fds[i].events |= POLLIN;
                logMessage(DEBUG,"sock linked [%s]:[%d]",client_ip.c_str(),client_port);
            }
            else
            {
                close(sockfd);
                logMessage(WARING,"accept sock error,full");
            }
        }
    }
    void Receive(int pos)
    {
        char buffer[1024];
        int n = recv(_fds[pos].fd,buffer,sizeof(buffer)-1,0);
        if(n < 0)
        {
            logMessage(WARING,"[%d] sock recv error",_fds[pos].fd);
            close(_fds[pos].fd);
            _fds[pos].fd = FD_NONE;
            _fds[pos].events = _fds[pos].revents = 0;
        }
        else if(n == 0)
        {
            logMessage(DEBUG,"[%d] client link closed",_fds[pos].fd);
            close(_fds[pos].fd);
            _fds[pos].fd = FD_NONE;
            _fds[pos].events = _fds[pos].revents = 0;
        }
        else
        {
            buffer[n] = 0;
            logMessage(DEBUG,"MESSAGE[%d]:%s", _fds[pos].fd, buffer);
        }
    }
    void DebugPrint()
    {
        for(int i = 0; i < NUM; i++)
        {
            if(~_fds[i].fd)
                std::cout << ~_fds[i].fd << " ";
        }    
        std::cout << std::endl;
    }
public:
    PollServer(uint16_t port = 8080)
    {
        _listensock = _sock.Socket();
        _sock.Bind(_listensock,port);
        _sock.Listen(_listensock);
        logMessage(DEBUG,"PollServer initiate success");
        _fds = new pollfd[NUM];
        for(int i = 0; i < NUM; i++)
        {
            _fds[i].fd = FD_NONE;
            _fds[i].events = _fds[i].revents =  0;
        }
        _fds[0].fd = _listensock;
        _fds[0].events |= POLLIN;
    }
    void Start()
    {
        int timeout = 3000;
        while(true)
        {
            DebugPrint();
            int n = poll(_fds,NUM,timeout);
            switch(n)
            {
                case 0:
                logMessage(DEBUG,"time out");
                break;
                case -1:
                logMessage(WARING,"Poll error[%d]:[%s]",errno,strerror(errno));
                break;
                default:
                HandlerEvent();
            }
        }
    }
    ~PollServer()
    {
        if(_listensock >= 0)
            close(_listensock);
        delete[] _fds;
    }
private:
    Sock _sock;
    int _listensock;
    pollfd* _fds;
};


#endif