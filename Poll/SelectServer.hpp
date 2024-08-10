#ifndef SELECT

#define SELECT
#include <sys/select.h>
#include "Sock.hpp"
#include <iostream>

#define BITS 8
#define NUM (sizeof(fd_set)*BITS)
#define FD_NONE -1

class SelectServer
{
private:
    void HanderEvent(fd_set fds)
    {
        if(FD_ISSET(_listensock,&fds))
        {
            Accept();
        }
        else
        {   
            for(int i = 1; i < NUM; i++)
            {
                if(FD_ISSET(_fd_array[i], &fds))
                {
                    Receive(i);
                }
            }
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
            int i = 1;
            for(; i < NUM; i++)
            {
                if(_fd_array[i] == FD_NONE)
                    break;
            }
            if(i < NUM)
            {
                _fd_array[i] = sockfd;
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
        int n = recv(_fd_array[pos],buffer,sizeof(buffer)-1,0);
        if(n < 0)
        {
            logMessage(WARING,"[%d] sock recv error",_fd_array[pos]);
            close(_fd_array[pos]);
            _fd_array[pos] = FD_NONE;
        }
        else if(n == 0)
        {
            logMessage(DEBUG,"[%d] client link closed",_fd_array[pos]);
            close(_fd_array[pos]);
            _fd_array[pos] = FD_NONE;
        }
        else
        {
            buffer[n] = 0;
            logMessage(DEBUG,"MESSAGE[%d]:%s", _fd_array, buffer);
        }
    }
    void DebugPrint()
    {
        for(int i = 0; i < NUM; i++)
        {
            if(~_fd_array[i])
                std::cout << _fd_array[i] << " ";
        }    
        std::cout << std::endl;
    }
public:
    SelectServer(uint16_t port = 8080)
    {
        for(int i = 0; i < NUM; i++)
        {
            _fd_array[i] = FD_NONE;
        }
        _listensock = _sock.Socket();
        _sock.Bind(_listensock,port);
        _sock.Listen(_listensock);
        _fd_array[0] = _listensock;
        logMessage(DEBUG,"server initiate success");
    }
    void Start()
    {
        while(true)
        {
            DebugPrint();
            
            timeval timeout = {3,0};
            fd_set fds;
            FD_ZERO(&fds);
            int maxfd = 0;
            for(int i = 0; i < NUM; i++)
            {
                if(_fd_array[i] != FD_NONE)
                {
                    FD_SET(_fd_array[i],&fds);
                    if(_fd_array[i] > maxfd)
                        maxfd = _fd_array[i];
                }
            }
            int n = select(maxfd+1,&fds,nullptr,nullptr,&timeout);
            switch(n)
            {
                case -1:
                logMessage(WARING,"select error");
                case 0:
                logMessage(DEBUG,"select timeout");
                default:
                HanderEvent(fds);
            }
        }
    }

    ~SelectServer()
    {
        if(_listensock >= 0)
            close(_listensock);
    }
private:
    int _listensock;
    Sock _sock;
    int _fd_array[NUM];
};


#endif